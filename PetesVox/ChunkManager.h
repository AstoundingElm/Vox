#pragma once



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
	
	void render( Shader * shader,  Camera * camera)
	{
		for (auto& chunk : m_chunks)
			chunk->Render(shader,  camera);
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