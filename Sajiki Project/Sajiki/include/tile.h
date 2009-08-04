#ifndef TILE_H
#define TILE_H
#include "asset.h"

namespace Sajiki
{
	/**
		@class Tile
		@brief Asset which is a basic level block.
	 */
	class Tile: public Asset
	{
		friend Asset;
	public:
		Tile(int x, int y);
		void Update(){};
	private:

	protected:
	};
}

#endif