// misc/shader.glsl -- GLSL shader for applying an exaggerated CRT filter effect to the game window.

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright 2025 Raine Simmons <gc@gravecat.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#version 130

uniform sampler2D tex;
uniform vec2 textureSize;
uniform float time;

uniform float bloomIntensity = 0.7;
uniform float bloomSpread = 2.0;
uniform float chromaticAberration = 0.8;
uniform float colorBleedAmount = 0.2;
uniform float curvature = 6.0;
uniform float flickerAmount = 0.03;
uniform float flickerSpeed = 3.0;
uniform float glitchFrequency = 0.002;
uniform float glitchMaxIntensity = 0.005;
uniform float glitchMaxSize = 1.0;
uniform float glitchMinIntensity = -0.005;
uniform float glitchMinSize = 1.0;
uniform float jitterIntensity = 0.0003;
uniform float jitterSpeed = 0.5;
uniform float noiseAmount = 0.015;
uniform float phosphorIntensity = 0.15;
uniform float phosphorScale = 1.5;
uniform float rollIntensity = 0.002;
uniform float rollSpeed = 0.01;
uniform float scanlineCount = 200.0;
uniform float scanlineDriftAmount = 0.05;
uniform float scanlineDriftSpeed = 0.1;
uniform float scanlineIntensity = 0.2;
uniform float vignetteIntensity = 0.23;

out vec4 fragColor;

precision lowp float;

float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

vec2 getDistortedUV(vec2 uv) {
    float verticalRoll = sin(time * rollSpeed + uv.y * 10.0) * rollIntensity;
    float horizontalJitter = (noise(vec2(time * jitterSpeed, 0.0)) * 2.0 - 1.0) * jitterIntensity;

    return uv + vec2(horizontalJitter, verticalRoll);
}

vec2 curveRemapUV(vec2 uv) {
    uv = uv * 2.0 - 1.0;
    vec2 offset = abs(uv.yx) / vec2(curvature);
    uv = uv + uv * offset * offset;
    uv = (uv + 1.0) * 0.5;
    return uv;
}

vec4 getBloom(vec2 texCoord) {
    vec4 sum = vec4(0.0);
    int samples = 3;

    for (int i = -samples; i <= samples; i++) {
        for (int j = -samples; j <= samples; j++) {
            vec2 offset = vec2(float(i), float(j)) * (bloomSpread / textureSize);
            sum += texture(tex, texCoord + offset);
        }
    }

    return sum / float((samples * 2 + 1) * (samples * 2 + 1));
}

float getVignette(vec2 uv) {
    // Transform UV to range [-1, 1]
    uv = (uv - 0.5) * 2.0;

    // Adjust for aspect ratio to stretch the vignette closer to the corners
    uv.x *= 1.5; // Stretch horizontally (adjust this value as needed)
    uv.y *= 1.2; // Stretch vertically (adjust this value as needed)

    // Calculate vignette intensity
    return 1.0 - dot(uv, uv) * vignetteIntensity;
}

// New function for phosphor mask
vec3 getPhosphorMask(vec2 uv) {
    vec2 phosphorUV = uv * textureSize * phosphorScale;
    vec3 mask = vec3(
        step(1.0, mod(phosphorUV.x, 3.0)),
        step(2.0, mod(phosphorUV.x, 3.0)),
        step(3.0, mod(phosphorUV.x, 3.0))
    );
    return mix(vec3(1.0), mask, phosphorIntensity);
}

// Enhanced noise function for grain
float getNoise(vec2 uv) {
    return random(uv + vec2(time * 0.001)) * 2.0 - 1.0;
}

// Function to generate random glitchy horizontal flicker
float getGlitchEffect(vec2 uv) {
    // Randomize glitch band position based on time and frequency
    float glitchBand = step(1.0 - glitchFrequency, random(vec2(time * 0.1, floor(uv.y * 10.0)))); // Random bands

    // Randomize glitch band size
    float bandSize = mix(glitchMinSize, glitchMaxSize, random(vec2(time * 0.2, floor(uv.y * 10.0))));
    glitchBand *= step(uv.y, bandSize); // Apply glitch only within the band size

    // Randomize glitch intensity
    float glitchStrength = mix(glitchMinIntensity, glitchMaxIntensity, random(vec2(time * 0.3, floor(uv.y * 10.0))));

    return glitchBand * glitchStrength;
}

void main() {
    vec2 originalUV = gl_TexCoord[0].xy;
    vec2 texCoordRemapped = curveRemapUV(originalUV);

    vec2 distortedTexCoord = getDistortedUV(texCoordRemapped);

    if (distortedTexCoord.x < 0.0 || distortedTexCoord.x > 1.0 || 
        distortedTexCoord.y < 0.0 || distortedTexCoord.y > 1.0) {
        distortedTexCoord = texCoordRemapped;
    }

    // Apply glitch effect
    float glitchOffset = getGlitchEffect(originalUV); // Get glitch offset for the current UV
    distortedTexCoord.x += glitchOffset; // Offset the horizontal UV coordinate

    // Chromatic aberration
    float aberrationAmount = length(vec2(0.5) - texCoordRemapped) * chromaticAberration;
    vec2 redOffset = vec2(aberrationAmount, 0.0) * 0.001;
    vec2 blueOffset = vec2(-aberrationAmount, 0.0) * 0.001;

    vec4 basePixelR = texture(tex, distortedTexCoord + redOffset);
    vec4 basePixelG = texture(tex, distortedTexCoord);
    vec4 basePixelB = texture(tex, distortedTexCoord + blueOffset);
    vec4 basePixel = vec4(basePixelR.r, basePixelG.g, basePixelB.b, basePixelG.a);

    vec4 bloom = getBloom(distortedTexCoord);
    vec4 pixel = basePixel + (bloom * bloomIntensity);

    float ghostOffset = sin(time * 1.5) * 0.001;
    vec4 ghostR = texture(tex, distortedTexCoord + vec2(ghostOffset, 0.0));
    vec4 ghostB = texture(tex, distortedTexCoord - vec2(ghostOffset, 0.0));
    pixel.r = mix(pixel.r, ghostR.r, colorBleedAmount);
    pixel.b = mix(pixel.b, ghostB.b, colorBleedAmount);

    // Scanlines with drift
    float scanlinePos = distortedTexCoord.y + (sin(time * scanlineDriftSpeed) * scanlineDriftAmount);
    float scanline = sin(scanlinePos * scanlineCount * 3.14159);
    scanline = (1.0 - scanlineIntensity) + scanlineIntensity * scanline;

    vec4 finalColor = pixel;

    // Apply phosphor mask
    finalColor.rgb *= getPhosphorMask(distortedTexCoord);

    // Apply scanline
    finalColor *= vec4(vec3(scanline), 1.0);

    // Apply vignette
    finalColor.rgb *= getVignette(texCoordRemapped);

    // Apply brightness flicker
    float flicker = 1.0 + (sin(time * flickerSpeed) * flickerAmount);
    finalColor.rgb *= flicker;

    // Add noise grain
    float noise = getNoise(distortedTexCoord) * noiseAmount;
    finalColor.rgb += noise;

    fragColor = finalColor;
}
