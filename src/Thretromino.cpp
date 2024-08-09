#include "Thretromino.hpp"
#include "Thretris.hpp"

void Thretromino::UpdateInWorld() {
	std::vector<glm::vec3> blockPositions;
	for(auto member : shape) {
		blockPositions.push_back(center + glm ::vec3(member));
	}

	if(blockPositions.size() != blocks.size()) {
		//Oh dear
		Logging::ClientLog("Oh noes mi thretromino is all screwed up oh gosh bye", LogLevel::Error);
		Engine::GetInstance()->Stop();
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
			ret->shape.emplace_back(0, 0, 0);
			ret->shape.emplace_back(0, 1, 0);
			ret->shape.emplace_back(0, 2, 0);
			ret->shape.emplace_back(0, 3, 0);
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
	ret->center = {0, 12.5f, 0};

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