#ifndef _ENTITY_PROPERTIES_H
#define _ENTITY_PROPERTIES_H


struct Entity
{
	float  x;
	float  y;
	float  width;
	float  height;
	float  entity_vertecies[12];
	int    entity_indecies[6];
	char	*vertex_shader_source;
	char	*fragment_shader_source;
	GLuint vertex_shader_ID;
	GLuint fragement_shader_ID;
	GLuint shader_program_ID;
	GLuint vertex_buffer_object;
	GLuint vertex_array_object;
	GLuint element_buffer_object;
	Entity(float _x = 0, float _y = 0, float Width = 0, float Height = 0, const char* vertex_shader = 0, const char* fragment_shader = 0)
	{
		// entity_vertecies = (float*)malloc(sizeof(float) * 12);
		// entity_indecies = (int*)malloc(sizeof(int) * 6);
		x = _x;
		y = _y;
		width = Width;
		height = Height;
		entity_vertecies[0] = -width + x;
		entity_vertecies[1] = height + y;
		entity_vertecies[2] = 0.0f;
		entity_vertecies[3] = width + x;
		entity_vertecies[4] = height + y;
		entity_vertecies[5] = 0.0f;
		entity_vertecies[6] = width + x;
		entity_vertecies[7] = -height + y;
		entity_vertecies[8] = 0.0f;
		entity_vertecies[9] = -width + x;
		entity_vertecies[10] = -height + y;
		entity_vertecies[11] = 0.0f;
		
		entity_indecies[0] = 0;
		entity_indecies[1] = 1;
		entity_indecies[2] = 3;
		entity_indecies[3] = 1;
		entity_indecies[4] = 3;
		entity_indecies[5] = 2;
		
		
		Buffer	vertex_shader_buffer = readFileNullTerminated(vertex_shader);
		
		Buffer	fragment_shader_buffer = readFileNullTerminated(fragment_shader);
		
		vertex_shader_ID = glCreateShader(0x8B31);
		glShaderSource(vertex_shader_ID, 1, &((const char*)vertex_shader_buffer.mem), 0);
		glCompileShader(vertex_shader_ID);
		fragement_shader_ID = glCreateShader(0x8B30);
		glShaderSource(fragement_shader_ID, 1, &((const char*)fragment_shader_buffer.mem), 0);
		glCompileShader(fragement_shader_ID);
		shader_program_ID = glCreateProgram();
		glAttachShader(shader_program_ID, vertex_shader_ID);
		glAttachShader(shader_program_ID, fragement_shader_ID);
		glLinkProgram(shader_program_ID);
		glDeleteShader(vertex_shader_ID);
		glDeleteShader(fragement_shader_ID);
		
		glGenVertexArrays(1, &vertex_array_object);
		glGenBuffers(1, &vertex_buffer_object);
		glGenBuffers(1, &element_buffer_object);
		
		glBindBuffer(0x8892, vertex_buffer_object);
		glBindVertexArray(vertex_array_object);
		glBufferData(0x8892, sizeof(entity_vertecies), entity_vertecies, 0x88E4);
		glBindBuffer(0x8893, element_buffer_object);
		glBufferData(0x8893, sizeof(entity_indecies), entity_indecies, 0x88E4);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}
	void render_and_update_entity()
	{
		entity_vertecies[0] = -width + x;
		entity_vertecies[1] = height + y;
		entity_vertecies[2] = 0.0f;
		entity_vertecies[3] = width + x;
		entity_vertecies[4] = height + y;
		entity_vertecies[5] = 0.0f;
		entity_vertecies[6] = width + x;
		entity_vertecies[7] = -height + y;
		entity_vertecies[8] = 0.0f;
		entity_vertecies[9] = -width + x;
		entity_vertecies[10] = -height + y;
		entity_vertecies[11] = 0.0f;
		
		glBindBuffer(0x8892, vertex_buffer_object);
		glBindVertexArray(vertex_array_object);
		glBufferData(0x8892, sizeof(entity_vertecies), entity_vertecies, 0x88E4);
		glUseProgram(shader_program_ID);	  
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
};


#endif //_ENTITY_PROPERTIES_H
