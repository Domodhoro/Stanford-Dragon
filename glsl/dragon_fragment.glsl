#version 300 es

out highp vec4      FragColor;
in highp vec3       Normal;
in highp vec3       FragPos;
uniform highp vec3  light_position;
uniform highp vec3  light_color;

highp vec3 light(const float specular_strength, const float shininess) {
    highp vec3  light_direction = normalize(light_position  - FragPos);
    highp float specular        = pow      (max(dot(light_direction, reflect(-light_direction, Normal)), 0.0f), shininess);
	highp float light_resultant = max      (dot(Normal, light_direction), 0.0f);

	return vec3(light_resultant) + (specular_strength * vec3(specular));
}

void main() {
    highp vec3 light = light(10.0f, 32.0f);
    FragColor        = vec4(light_color * light, 1.0f);
}
