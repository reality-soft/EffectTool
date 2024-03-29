#define e 2.71828182846
#define EPSILON 0.001f
// Global Directional Lighting
cbuffer CbGlobalLight : register(b0)
{
    float4 gblight_pos;
    float4 gblight_color;
    
    float3 gblight_dir;
    float specular_strength;
    
    float4 ambient_up;
    float4 ambient_down;
    float4 ambient_range;
}

// Point Lighting
struct PointLight
{
    float4  light_color;
    float3  position;
    float   range;
    float3  attenuation;
    float   specular;
};

cbuffer CbPointLights : register(b2)
{
    PointLight point_lights[64];
}

// Spot Lighting
struct SpotLight
{
    float4  light_color;
    float3  position;
    float   range;
    float3  attenuation;
    float   specular;
    float3  direction;
    float   spot;
};

cbuffer CbSpotLights : register(b3)
{
    SpotLight spot_lights[64];
}

cbuffer CbDistanceFog : register(b4)
{
    float4 fog_color;
    float3 eye_position;
    float fog_distance;
}

// White Basic color  
float4 WhiteColor()
{
    return float4(1, 1, 1, 1);
}
  
// Returns Color with Texture, returns white color if no texture
float4 CreateColor(Texture2D tex, SamplerState sample, float2 uv)
{
    float4 color = tex.Sample(sample, uv);
    if (length(color) == 0)
        return WhiteColor();
    
    return color;
}

float3 RGBtoHSV(float3 RGB)
{
    float3 HSV = 0;
    float M = min(RGB.r, min(RGB.g, RGB.b));
    HSV.z = max(RGB.r, max(RGB.g, RGB.b));
    float C = HSV.z - M;
    if (C != 0)
    {
        HSV.y = C / HSV.z;
        float3 D = (((HSV.z - RGB) / 6) + (C / 2)) / C;
        if (RGB.r == HSV.z)
            HSV.x = D.b - D.g;
        else if (RGB.g == HSV.z)
            HSV.x = (1.0 / 3.0) + D.r - D.b;
        else if (RGB.b == HSV.z)
            HSV.x = (2.0 / 3.0) + D.g - D.r;
        if (HSV.x < 0.0)
        {
            HSV.x += 1.0;
        }
        if (HSV.x > 1.0)
        {
            HSV.x -= 1.0;
        }
    }
    return HSV;
}

float3 HSVtoRGB(float3 HSV)
{
    float3 RGB = 0;
    float C = HSV.z * HSV.y;
    float H = HSV.x * 6;
    float X = C * (1 - abs(fmod(H, 2) - 1));
    if (HSV.y != 0)
    {
        float I = floor(H);
        if (I == 0)
        {
            RGB = float3(C, X, 0);
        }
        else if (I == 1)
        {
            RGB = float3(X, C, 0);
        }
        else if (I == 2)
        {
            RGB = float3(0, C, X);
        }
        else if (I == 3)
        {
            RGB = float3(0, X, C);
        }
        else if (I == 4)
        {
            RGB = float3(X, 0, C);
        }
        else
        {
            RGB = float3(C, 0, X);
        }
    }
    float M = HSV.z - C;
    return RGB + M;
}

float4 ChangeHue(float4 color, float amount)
{
    float3 hsv = RGBtoHSV(color.xyz);
    hsv.x *= amount;
    return float4(HSVtoRGB(hsv), 1.0f);
}

float4 ChangeSaturation(float4 color, float amount)
{
    float3 hsv = RGBtoHSV(color.xyz);
    hsv.y *= amount;
    return float4(HSVtoRGB(hsv), 1.0f);
}

float4 ChangeValue(float4 color, float amount)
{
    float3 hsv = RGBtoHSV(color.xyz);
    hsv.z *= amount;
    return float4(HSVtoRGB(hsv), 1.0f);
}

float4 ApplyHemisphericAmbient(float3 normal, float4 color)
{
    // Convert from [-1, 1] to [0, 1]
    float up = normal * ambient_range.xyz + ambient_range.xyz;
    // Calculate the ambient value
    float3 ambient = ambient_down + up * ambient_up;

    // Apply the ambient value to the color
    return length(ambient) * color;
}

float4 ApplyCookTorrance(float4 albedo, float roughness, float3 specular, float3 normal, float3 view_dir)
{
    // Correct the input and compute aliases
    view_dir = normalize(view_dir);
    float3 light_dir = normalize(-gblight_dir);
    float3 half_vec = normalize(light_dir + view_dir);
    float normal_dot_half = dot(normal, half_vec);
    float view_dot_half = dot(half_vec, view_dir);
    float normal_dot_view = dot(normal, view_dir);
    float normal_dot_light = dot(normal, light_dir);

    // Compute the geometric term  
    float G1 = (2.0f * normal_dot_half * normal_dot_view) / view_dot_half;
    float G2 = (2.0f * normal_dot_half * normal_dot_light) / view_dot_half;
    float G = min(1.0f, max(0.0f, min(G1, G2)));

    // Compute the fresnel term
    float F = roughness + (1.0f - roughness) * pow(1.0f - normal_dot_view, 5.0f);

    // Compute the roughness term  
    float R_2 = roughness * roughness;
    float NDotH_2 = normal_dot_half * normal_dot_half;
    float A = 1.0f / (4.0f * R_2 * NDotH_2 * NDotH_2);
    float B = exp(-(1.0f - NDotH_2) / (R_2 * NDotH_2));
    float R = A * B;

    // Compute the final term  
    float3 S = ((G * F * R) / (normal_dot_light * normal_dot_view)) * specular;
    float3 flinal_color = gblight_color.xyz * max(0.2f, normal_dot_light) * (albedo.xyz + S);
    return float4(flinal_color, 1.0f);
}

float4 ApplyPointLight(float4 color, float3 normal, float3 world_pos, float3 view_dir)
{
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float4 specular = float4(0.0f, 0.0f, 0.0f, 1.0f);

    for (int i = 0; i < 64; i++)
    {
        if (point_lights[i].range == 0.0f)
            continue;

        float3 lightVec = point_lights[i].position - world_pos;

        float distance = length(lightVec);

        if (distance > point_lights[i].range)
            continue;
        else
        {
            lightVec = normalize(lightVec);

            float intensity = max(dot(lightVec, normalize(normal)), 0.0f);
            float att = 1.0f / dot(point_lights[i].attenuation, float3(1.0f, distance, distance * distance));

            diffuse += point_lights[i].light_color * intensity * att;

            float3 v = reflect(-lightVec, normal);
            float specFactor = pow(max(dot(v, view_dir), 0.0f), 0.1f);
            float4 spec = specFactor * spot_lights[i].specular * att;

            specular += spec;
        }

    }

    return ApplyCookTorrance(color * diffuse, 0.6f, specular, normal, view_dir);
}

float4 ApplySpotLight(float4 color, float3 normal, float3 world_pos, float3 view_dir)
{
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 64; i++)
    {
        if (spot_lights[i].range == 0.0f)
            continue;

        float3 lightVec = spot_lights[i].position - world_pos;

        float distance = length(lightVec);

        if (distance > spot_lights[i].range)
            continue;

        lightVec = normalize(lightVec);

        float intensity = max(dot(lightVec, normalize(normal)), 0.0f);

        float spot = pow(max(dot(-lightVec, spot_lights[i].direction), 0.0f), spot_lights[i].spot);

        float att = spot / dot(spot_lights[i].attenuation, float3(1.0f, distance, distance * distance));

        diffuse += spot_lights[i].light_color * intensity * att;

        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, view_dir), 0.0f), 0.1f);
        float4 spec = specFactor * spot_lights[i].specular * att;

        specular += spec;
    }

    return ApplyCookTorrance(color * diffuse, 0.6f, specular, normal, view_dir);
}

float4 ApplyDistanceFog(float4 color, float3 pixel_world)
{
    float3 fog_start = eye_position;
    float3 fog_end = normalize(pixel_world - fog_start) * fog_distance;

    float f = 1 / pow(e, pow(length(pixel_world - fog_start) / fog_distance, 2));    
    return f * color + (1.0f - f) * fog_color;
}