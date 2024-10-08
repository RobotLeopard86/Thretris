#pragma once

#include "Cacao.hpp"

using namespace Cacao;

#include "BlkMatOpt.hpp"

struct Thretromino {
  public:
	std::map<std::shared_ptr<Entity>, std::shared_ptr<MeshComponent>> blocks;
	BlkMatOpt color;
	glm::vec3 center;
	std::vector<std::vector<glm::i8vec3>> shapes;
	int idx;
	void UpdateInWorld();
	void Freeze();
	~Thretromino();
	bool frozen;
	bool quickDropped;

	Thretromino()
	  : frozen(false), quickDropped(false) {}
};

enum class ThretrominoType {
	Star,
	Bar,
	Box,
	Ring,
	H,
	T,
	Table,
	C,
	Bit
};

std::shared_ptr<Thretromino> SpawnThretromino(ThretrominoType tp);