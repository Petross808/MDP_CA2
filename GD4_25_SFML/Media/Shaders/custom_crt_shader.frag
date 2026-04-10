//Baseline code generated with support of https://chatgpt.com/
//Values and small fixes adjusted by Jakub Polacek D00260171

uniform sampler2D texture;
uniform vec2 pixelSize = vec2(320.0, 180.0);
uniform vec2 screenCenter = vec2(0.5, 0.5);

void main()
{
    // Calculate distance from the screen center
    vec2 coord = gl_TexCoord[0].xy;
    vec2 offset = coord - screenCenter;
    float distance = length(offset);

    // Apply radial distortion (CRT-like effect)
    float distortionFactor = 0.15;
    vec2 distortedCoord = coord + offset * distortionFactor * (distance * distance);

    // Pixelate effect: Round coordinates to nearest block
    distortedCoord = floor(distortedCoord * pixelSize) / pixelSize;
    vec4 pixel = texture2D(texture, distortedCoord);

    //  Scanline effect
    float scanline = sin(gl_TexCoord[0].y * 200.0) * 0.05;
    pixel.rgb -= scanline;

    //  Make black corners
    float cornerThreshold = 0.7;
    if (distance > cornerThreshold) {
        pixel.rgb = vec3(0.0);
    }

    // Vignette Effect
    float vignetteStart = 0.5;
    float vignetteEnd = 0.7;
    float vignette = smoothstep(vignetteStart, vignetteEnd, distance);
    pixel.rgb *= mix(1.0, 0, vignette);


    // 5. Output the final color
    gl_FragColor = pixel;
}