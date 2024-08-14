#include "Thretromino.hpp"
#include "Thretris.hpp"

void Thretromino::UpdateInWorld() {
	if(frozen) {
		Logging::ClientLog("Bruh i'm frozen why u tryna update me?", LogLevel::Error);
		return;
	}

	std::vector<glm::vec3> blockPositions;
	for(auto member : shape) {
		glm::vec3 boardSpace = center + glm::vec3(member);
		blockPositions.emplace_back(boardSpace.x + 5, boardSpace.y - 10, boardSpace.z - 5);
	}

	if(blockPositions.size() != blocks.size()) {
		Logging::ClientLog("Oh noes mi thretromino is all screwed up oh gosh oh man", LogLevel::Error);
		return;
	}

	int count = 0;
	for(auto b : blocks) {
		switch(color) {
			case BlkMatOpt::Red: {
				b.second->mat = Thretris::GetInstance()->pub.redM;
				break;
			}
			case BlkMatOpt::Orn: {
				b.second->mat = Thretris::GetInstance()->pub.orangeM;
				break;
			}
			case BlkMatOpt::Gld: {
				b.second->mat = Thretris::GetInstance()->pub.goldM;
				break;
			}
			case BlkMatOpt::Grn: {
				b.second->mat = Thretris::GetInstance()->pub.greenM;
				break;
			}
			case BlkMatOpt::Cyn: {
				b.second->mat = Thretris::GetInstance()->pub.cyanM;
				break;
			}
			case BlkMatOpt::Lbl: {
				b.second->mat = Thretris::GetInstance()->pub.lblueM;
				break;
			}
			case BlkMatOpt::Dbl: {
				b.second->mat = Thretris::GetInstance()->pub.dblueM;
				break;
			}
			case BlkMatOpt::Prp: {
				b.second->mat = Thretris::GetInstance()->pub.purpleM;
				break;
			}
			case BlkMatOpt::Pnk: {
				b.second->mat = Thretris::GetInstance()->pub.pinkM;
				break;
			}
			default: break;
		}
		b.first->SetActive(true);
		b.first->GetLocalTransform().SetPosition(blockPositions[count]);
		count++;
	}
}

std::shared_ptr<Thretromino> SpawnThretromino(ThretrominoType tp) {
	std::shared_ptr<Thretromino> ret = std::make_shared<Thretromino>();

	switch(tp) {
		case ThretrominoType::Bar: {
			ret->color = BlkMatOpt::Lbl;
			ret->shape.emplace_back(0, -1, 0);
			ret->shape.emplace_back(0, 0, 0);
			ret->shape.emplace_back(0, 1, 0);
			break;
		}
		case ThretrominoType::Box: {
			ret->color = BlkMatOpt::Orn;
			ret->shape.emplace_back(-1, -1, -1);
			ret->shape.emplace_back(0, -1, -1);
			ret->shape.emplace_back(1, -1, -1);
			ret->shape.emplace_back(1, -1, 0);
			ret->shape.emplace_back(1, -1, 1);
			ret->shape.emplace_back(0, -1, 1);
			ret->shape.emplace_back(-1, -1, 1);
			ret->shape.emplace_back(-1, -1, 0);
			ret->shape.emplace_back(0, -1, 0);
			ret->shape.emplace_back(-1, 1, -1);
			ret->shape.emplace_back(0, 1, -1);
			ret->shape.emplace_back(1, 1, -1);
			ret->shape.emplace_back(1, 1, 0);
			ret->shape.emplace_back(1, 1, 1);
			ret->shape.emplace_back(0, 1, 1);
			ret->shape.emplace_back(-1, 1, 1);
			ret->shape.emplace_back(-1, 1, 0);
			ret->shape.emplace_back(0, 1, 0);
			ret->shape.emplace_back(-1, 0, -1);
			ret->shape.emplace_back(0, 0, -1);
			ret->shape.emplace_back(1, 0, -1);
			ret->shape.emplace_back(1, 0, 0);
			ret->shape.emplace_back(1, 0, 1);
			ret->shape.emplace_back(0, 0, 1);
			ret->shape.emplace_back(-1, 0, 1);
			ret->shape.emplace_back(-1, 0, 0);
			ret->shape.emplace_back(0, 0, 0);
			break;
		}
		case ThretrominoType::C: {
			ret->color = BlkMatOpt::Grn;
			ret->shape.emplace_back(1, 1, 0);
			ret->shape.emplace_back(0, 1, 0);
			ret->shape.emplace_back(-1, 1, 0);
			ret->shape.emplace_back(-1, 0, 0);
			ret->shape.emplace_back(-1, -1, 0);
			ret->shape.emplace_back(0, -1, 0);
			ret->shape.emplace_back(1, -1, 0);
			break;
		}
		case ThretrominoType::H: {
			ret->color = BlkMatOpt::Pnk;
			ret->shape.emplace_back(-1, 1, 0);
			ret->shape.emplace_back(-1, 0, 0);
			ret->shape.emplace_back(-1, -1, 0);
			ret->shape.emplace_back(1, 1, 0);
			ret->shape.emplace_back(1, 0, 0);
			ret->shape.emplace_back(1, -1, 0);
			ret->shape.emplace_back(0, 0, 0);
			break;
		}
		case ThretrominoType::I: {
			ret->color = BlkMatOpt::Red;
			ret->shape.emplace_back(0, 1, 1);
			ret->shape.emplace_back(0, 1, 0);
			ret->shape.emplace_back(0, 1, -1);
			ret->shape.emplace_back(0, -1, 1);
			ret->shape.emplace_back(0, -1, 0);
			ret->shape.emplace_back(0, -1, -1);
			ret->shape.emplace_back(0, 0, 0);
			break;
		}
		case ThretrominoType::Ring: {
			ret->color = BlkMatOpt::Cyn;
			ret->shape.emplace_back(-1, 0, -1);
			ret->shape.emplace_back(0, 0, -1);
			ret->shape.emplace_back(1, 0, -1);
			ret->shape.emplace_back(1, 0, 0);
			ret->shape.emplace_back(1, 0, 1);
			ret->shape.emplace_back(0, 0, 1);
			ret->shape.emplace_back(-1, 0, 1);
			ret->shape.emplace_back(-1, 0, 0);
			break;
		}
		case ThretrominoType::Star: {
			ret->color = BlkMatOpt::Gld;
			ret->shape.emplace_back(0, 0, 0);
			ret->shape.emplace_back(-1, 0, 0);
			ret->shape.emplace_back(1, 0, 0);
			ret->shape.emplace_back(0, 0, 1);
			ret->shape.emplace_back(0, 0, -1);
			ret->shape.emplace_back(0, -1, 0);
			ret->shape.emplace_back(0, 1, 0);
			break;
		}
		case ThretrominoType::T: {
			ret->color = BlkMatOpt::Prp;
			ret->shape.emplace_back(0, 1, 1);
			ret->shape.emplace_back(0, 1, 0);
			ret->shape.emplace_back(0, 1, -1);
			ret->shape.emplace_back(0, -1, 0);
			ret->shape.emplace_back(0, 0, 0);
			break;
		}
		case ThretrominoType::Table: {
			ret->color = BlkMatOpt::Dbl;
			ret->shape.emplace_back(-1, 1, -1);
			ret->shape.emplace_back(0, 1, -1);
			ret->shape.emplace_back(1, 1, -1);
			ret->shape.emplace_back(1, 1, 0);
			ret->shape.emplace_back(1, 1, 1);
			ret->shape.emplace_back(0, 1, 1);
			ret->shape.emplace_back(-1, 1, 1);
			ret->shape.emplace_back(-1, 1, 0);
			ret->shape.emplace_back(0, 1, 0);
			ret->shape.emplace_back(-1, 0, -1);
			ret->shape.emplace_back(-1, -1, -1);
			ret->shape.emplace_back(1, 0, -1);
			ret->shape.emplace_back(1, -1, -1);
			ret->shape.emplace_back(-1, 0, 1);
			ret->shape.emplace_back(-1, -1, 1);
			ret->shape.emplace_back(1, 0, 1);
			ret->shape.emplace_back(1, -1, 1);
			break;
		}
		default: break;
	}
	ret->center = {0, 20.0f, 0};

	for(int i = 0; i < ret->shape.size(); i++) {
		std::shared_ptr<Entity> ent = std::make_shared<Entity>(std::string("Platypus-") + std::to_string(i));
		std::shared_ptr<MeshComponent> mc = ent->GetComponent<MeshComponent>(ent->MountComponent<MeshComponent>());
		mc->mesh = Thretris::GetInstance()->pub.block;
		mc->SetActive(true);
		ent->SetActive(true);
		ent->SetParent(WorldManager::GetInstance()->GetWorld("Game").rootEntity);
		ent->GetLocalTransform().SetScale(glm::vec3(0.5f));
		ret->blocks.insert_or_assign(ent, mc);
	}

	ret->UpdateInWorld();

	return ret;
}

void Thretromino::Freeze() {
	if(frozen) {
		Logging::ClientLog("Bruh why u tryna freeze me againn?", LogLevel::Error);
		return;
	}

	glm::i8vec3 centerPos = {round(center.x + 5), round(center.y - 10), round(center.z - 5)};
	std::vector<glm::i8vec3> blockPositions;
	for(glm::i8vec3 member : shape) {
		glm::i8vec3 pos = centerPos + member;
		if(pos.x < 5 || pos.x > 14 || pos.z < -5 || pos.z > 4 || pos.y < -10 || pos.y > 9) {
			Logging::ClientLog("AYO WHAT THE SIGMA Y MI THRETROMINO OUT OF BOUNDS GRRRRR", LogLevel::Error);
			return;
		}
		if(Thretris::GetInstance()->blks[pos.x - 5][9 - (pos.z + 5)][pos.y + 10]) {
			Logging::ClientLog("AYO WHAT THE SIGMA Y SOMETHING IN ME WAY AAAAAGGGHHHHH", LogLevel::Error);
			return;
		}
		blockPositions.emplace_back(pos);
	}

	int c = 0;
	for(auto blk : blocks) {
		glm::i8vec3 p = blockPositions[c];
		blk.first->GetLocalTransform().SetPosition({float(p.x), float(p.y), float(p.z)});
		Thretris::GetInstance()->blks[p.x - 5][9 - (p.z + 5)][p.y + 10] = blk.first;
		c++;
	}
	frozen = true;
}

Thretromino::~Thretromino() {
	if(!frozen) {
		for(auto ent : blocks) {
			ent.first->SetParent(ent.first);
		}
	}
}