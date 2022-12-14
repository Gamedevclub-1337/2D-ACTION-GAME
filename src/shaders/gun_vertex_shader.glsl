#version 330 core
	  layout (location = 0) in vec3 aPos;
	  uniform float angle;
	  uniform float rotation_offset_x; 
	  uniform float rotation_offset_y;
	  void main()
	  {
	  float rotation_position_x = cos(angle) * (aPos.x - rotation_offset_x) - sin(angle) * (aPos.y - rotation_offset_y);
	   float rotation_position_y = sin(angle) * (aPos.x - rotation_offset_x) + cos(angle) * (aPos.y - rotation_offset_y);
	   rotation_position_x += rotation_offset_x;
	   rotation_position_y += rotation_offset_y;
	   gl_Position = vec4(rotation_position_x, rotation_position_y, aPos.z, 1.0);
}