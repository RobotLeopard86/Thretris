#pragma once

#include "Cacao.hpp"

using namespace Cacao;

#include "BlkMatOpt.hpp"

struct Thretromino {
  public:
	std::map<std::shared_ptr<Entity>, std::shared_ptr<MeshComponent>> blocks;
	BlkMatOpt color;
	glm::vec3 center;
	std::vector<glm::i8vec3> shape;
	void UpdateInWorld();
	void Freeze();
	~Thretromino();
	bool frozen;

	Thretromino()
	  : frozen(false) {}
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
	I
};

std::shared_ptr<Thretromino> SpawnThretromino(ThretrominoType tp);