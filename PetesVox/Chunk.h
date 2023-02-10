#pragma once

const int Width = 16, Height = 16, Depth = 16;

struct Chunk
{
    
	std::uint8_t m_blocks[Width][Height][Depth];
	
	Entity m_entity;
	
	Chunk()
	{
		// Fill in the chunk with blocks
		for (int x = 0; x < Width; x++)
		{
			for (int y = 0; y < Height; y++)
			{
				for (int z = 0; z < Depth; z++)
				{
					m_blocks[x][y][z] = 1;
				}
			}
		}
		
		// Generate the mesh
		generateChunkMesh();
	}
	
	~Chunk()
	{
	}
	
	void Update()
	{
		generateChunkMesh();
	}
	
	void Render(Shader * shader, Texture * texture, Camera * camera)
	{
		bind(shader);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->texture);
		
		loadMatrix(getUniformLocation(shader,"viewMatrix"), createViewMatrix(camera));
		
		m_entity.VAO.Bind();
		
		loadMatrix(getUniformLocation(shader, "transformationMatrix"), createTransformationMatrix(m_entity.position, m_entity.rotation, m_entity.scale));
		
		glDrawElements(GL_TRIANGLES, m_entity.EBO.size, GL_UNSIGNED_INT, 0);
		m_entity.VAO.Unbind();
		
		unbind();
	}
	
	std::uint8_t getBlock(int x, int y, int z)
	{
		return m_blocks[x][y][z];
	}
	
	void setBlock(int x, int y, int z, std::uint8_t type)
	{
		m_blocks[x][y][z] = type;
	}
	
	void setPosition(float x, float y, float z)
	{
		m_entity.position = glm::vec3(x, y, z);
	}
	
	glm::vec3 getPosition()
	{
		return m_entity.position;
	}
	
	Entity * getEntity()
	{
		return &m_entity;
	}
	
	void generateChunkMesh()
	{
		// Generate mesh by only drawing the faces if there aren't any blocks next to them
		std::vector<GLfloat> temp_verticies;
		std::vector<GLuint> temp_indicies;
		std::vector<GLfloat> temp_textureCoords;
		int indicieCount = 0;
		
		for (int x = 0; x < Width; x++)
		{
			for (int y = 0; y < Height; y++)
			{
				for (int z = 0; z < Depth; z++)
				{
					// Create block face data
					auto createFace = [&]( Face face, int x, int y, int z)
					{
						for (int i = 0; i < face.verticies.size() / 3; i++)
						{
							temp_verticies.push_back(face.verticies[0 + i * 3] + x); 
							temp_verticies.push_back(face.verticies[1 + i * 3] + y);
							temp_verticies.push_back(face.verticies[2 + i * 3] + z);
						}
						
						temp_textureCoords.push_back(0);
						temp_textureCoords.push_back(0);
						
						temp_textureCoords.push_back(0);
						temp_textureCoords.push_back(1);
						
						temp_textureCoords.push_back(1);
						temp_textureCoords.push_back(1);
						
						temp_textureCoords.push_back(1);
						temp_textureCoords.push_back(0);
						
						temp_indicies.push_back(indicieCount);
						temp_indicies.push_back(indicieCount + 1);
						temp_indicies.push_back(indicieCount + 3);
						temp_indicies.push_back(indicieCount + 3);
						temp_indicies.push_back(indicieCount + 1);
						temp_indicies.push_back(indicieCount + 2);
						
						indicieCount += 4;
					};
					
					// Check if the block is Air, if it is no need to draw it
					if (!m_blocks[x][y][z])
						continue;
					
					// Draw the faces on the end of the chunk
					if (x - 1 < 0)       createFace(getCubeFace( CubeFace::LEFT  ), x, y, z);
					if (x + 1 >= Width)  createFace(getCubeFace( CubeFace::RIGHT ), x, y, z);
					if (y - 1 < 0)       createFace(getCubeFace( CubeFace::BOTTOM), x, y, z);
					if (y + 1 >= Height) createFace( getCubeFace( CubeFace::TOP   ), x, y, z);
					if (z - 1 < 0)       createFace( getCubeFace( CubeFace::BACK  ), x, y, z);
					if (z + 1 >= Depth)  createFace( getCubeFace( CubeFace::FRONT ), x, y, z);
					
					// Draw faces only if there aren't blocks that aren't air next to them
					if (x - 1 >= 0 && !m_blocks[x - 1][y][z])     createFace( getCubeFace( CubeFace::LEFT),   x, y, z);
					if (x + 1 < Width && !m_blocks[x + 1][y][z])  createFace( getCubeFace( CubeFace::RIGHT),  x, y, z);
					if (y - 1 >= 0 && !m_blocks[x][y - 1][z])     createFace( getCubeFace( CubeFace::BOTTOM), x, y, z);
					if (y + 1 < Height && !m_blocks[x][y + 1][z]) createFace( getCubeFace( CubeFace::TOP),    x, y, z);
					if (z - 1 >= 0 && !m_blocks[x][y][z - 1])     createFace( getCubeFace( CubeFace::BACK),   x, y, z);
					if (z + 1 < Depth && !m_blocks[x][y][z + 1])  createFace( getCubeFace( CubeFace::FRONT),  x, y, z);
				}
			}
		}
		
		// Fill in the entity
		m_entity.setEBO(temp_indicies, GL_DYNAMIC_DRAW);
		m_entity.setVBO(temp_verticies, 0, 3, GL_DYNAMIC_DRAW);
		m_entity.setVBO(temp_textureCoords, 1, 2, GL_DYNAMIC_DRAW);
	}
	
};

struct ChunkManager
{
	
    std::vector<Chunk*> m_chunks;
	
    glm::uvec3 m_chunksSize;
	
    
	
	ChunkManager()
	{
	}
	
	~ChunkManager()
	{
		printf("ChunkManager: Destructor called!");
		
		for (auto& chunk : m_chunks)
			delete chunk;
	}
	
	void generate(int width, int height, int depth)
	{
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				for (int z = 0; z < depth; z++)
				{
					// Create the chunks
					auto* temp_chunk = new Chunk();
					temp_chunk->setPosition(x * Width, y * Height, z * Depth);
					
					m_chunks.push_back(temp_chunk);
				}
			}
		}
		
		m_chunksSize = glm::uvec3(width, height, depth);
	}
	
	void render( Shader * shader, Texture * texture, Camera * camera)
	{
		for (auto& chunk : m_chunks)
			chunk->Render(shader, texture, camera);
	}
	
	std::uint8_t getBlock(int x, int y, int z)
	{
		// First get which chunk it is
		int dx = x / Width;
		int dy = y / Height;
		int dz = z / Depth;
		
		int index = Convert3Dto1D(dx, dy, dz);
		auto chunk = m_chunks.at(index);
		
		// Then calculate which block it is
		int sx = x % Width;
		int sy = y % Height;
		int sz = z % Depth;
		
		return chunk->getBlock(sx, sy, sz);;
	}
	
	void setBlock(int x, int y, int z, std::uint8_t type)
	{
		// First get which chunk it is
		int dx = x / Width;
		int dy = y / Height;
		int dz = z / Depth;
		
		int index = Convert3Dto1D(dx, dy, dz);
		auto chunk = m_chunks.at(index);
		
		// Then calculate which block it is
		int sx = x %  Width;
		int sy = y %  Height;
		int sz = z %  Depth;
		
		chunk->setBlock(sx, sy, sz, type);
	}
	
	Chunk * getChunkFromWorldPosition(int x, int y, int z)
	{
		int dx = x /  Width;
		int dy = y /  Height;
		int dz = z /  Depth;
		
		int index = Convert3Dto1D(dx, dy, dz);
		return m_chunks.at(index);
	}
	
	Chunk * getChunkFromArray(int x, int y, int z)
	{
		int index = Convert3Dto1D(x, y, z);
		return m_chunks.at(index);
	}
	
	glm::uvec3 getChunksSize()
	{
		return m_chunksSize;
	}
	
	int Convert3Dto1D(float x, float y, float z)
	{
		// Index = ((x * YSIZE + y) * ZSIZE) + z;
		int WIDTH  = m_chunksSize.x;
		int HEIGHT = m_chunksSize.y;
		int DEPTH  = m_chunksSize.z;
		
		return ((x * HEIGHT + y) * DEPTH) + z;
	}
	
};