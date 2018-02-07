#include <sc2api/sc2_api.h>

#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <thread>

using namespace sc2;

class Bot : public Agent {
public:
	int gameloop;
	int ArmySize = 14;
	Unit scout;
	Units Minerals;
	BOOLEAN BuildWorkers = false;
	BOOLEAN BuildSupply = false;
	boost::system::error_code ignored_error;
	boost::asio::ip::tcp::socket *sock;

	void readThread() {
		boost::asio::streambuf response;
		std::istream response_stream(&response);
		std::string repo;
		std::string compare;
		while (true) {
			boost::asio::read_until(*sock, response, '\n');
			response_stream >> repo;
			std::cout << repo << std::endl;
			compare = repo.substr(0, 1);
			if (compare == "0") {
				BuildWorkers = false;
			}
			else if(compare=="1"){
				BuildWorkers = true;
			}
			else {
				BuildSupply = true;
			}
			response.consume(response.size());
		}
	}
	void OnGameStart() final {
		std::thread t1 (&Bot::readThread,this);
		t1.detach();
	}
	void OnGameEnd() final {
		if (Observation()->GetArmyCount() == 0) {
			std::cout << "I Lose." << std::endl;
			gameloop = -1;
		}
		else {
			std::cout << Observation()->GetGameLoop() << std::endl;
			gameloop = Observation()->GetGameLoop();
		}
	}
	virtual void OnStep() {
		if (Observation()->GetGameLoop() % 10 == 0) {
			std::string message = "0," + std::to_string(Observation()->GetMinerals()) + "\n";
			boost::asio::write(*sock, boost::asio::buffer(message), ignored_error);
			message = "1," + std::to_string(CountUnitType(sc2::UNIT_TYPEID::TERRAN_SCV)) + "\n";
			boost::asio::write(*sock, boost::asio::buffer(message), ignored_error);
			if (BuildWorkers) {
				BuildW();
			}
			if (BuildSupply) {
				if (TryBuildSupplyDepot()) {
					BuildSupply = false;
				}
			}
			
		}

		
	}

	virtual void OnUnitIdle(const Unit& unit) final {
		
	}
private:
	size_t CountUnitType(UNIT_TYPEID unit_type) {
		return Observation()->GetUnits(Unit::Alliance::Self, IsUnit(unit_type)).size();
	}
	
	void BuildW() {
		for (auto& x : Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_COMMANDCENTER))) {
			if (x->orders.empty()) {
				Actions()->UnitCommand(x, sc2::ABILITY_ID::TRAIN_SCV);
			}
		}
	}
	bool TryBuildSupplyDepot() {
		const ObservationInterface* observation = Observation();

		// If we are not supply capped, don't build a supply depot.
		//if (observation->GetFoodUsed() <= observation->GetFoodCap() *.85)
			//return false;


		if (observation->GetMinerals() < 100) {
			return false;
		}
		// Try and build a depot. Find a random SCV and give it the order.
		return TryBuildStructure(ABILITY_ID::BUILD_SUPPLYDEPOT);
	}
	bool TryBuildStructure(ABILITY_ID ability_type_for_structure, UNIT_TYPEID unit_type = UNIT_TYPEID::TERRAN_SCV) {
		const ObservationInterface* observation = Observation();

		// If a unit already is building a supply structure of this type, do nothing.
		// Also get an scv to build the structure.
		const Unit *unit_to_build=NULL;
		Units units = observation->GetUnits(Unit::Alliance::Self,IsUnit(unit_type));
		for (const auto& unit : units) {
			

			if (unit->unit_type == unit_type) {
				unit_to_build = unit;
			}
		}

		float rx = GetRandomScalar();
		float ry = GetRandomScalar();
		Actions()->UnitCommand(unit_to_build,
			ability_type_for_structure,
			Point2D(unit_to_build->pos.x + rx * 20.0f, unit_to_build->pos.y + ry * 20.0f),false);
		//if (unit_to_build->orders.empty() || unit_to_build->orders[0].ability_id !=ability_type_for_structure) {
			//return false;
		//}

		return true;
	}
	
};

int main(int argc, char* argv[]) {
	
	boost::asio::io_context io_context;
	boost::asio::ip::tcp tcp = boost::asio::ip::tcp::v4();
	boost::asio::ip::tcp::endpoint end = boost::asio::ip::tcp::endpoint(tcp, 4998);
	boost::asio::ip::tcp::acceptor acceptor= boost::asio::ip::tcp::acceptor(io_context, end);
	boost::asio::ip::tcp::socket sock = boost::asio::ip::tcp::socket(io_context);
	acceptor.accept(sock);
	std::string message = "0,100\n";
	boost::system::error_code ignored_error;
	boost::asio::write(sock, boost::asio::buffer(message), ignored_error);

	Coordinator coordinator;
	coordinator.LoadSettings(argc, argv);

	Bot bot;
	bot.sock = &sock;
	coordinator.SetParticipants({
		CreateParticipant(Race::Terran, &bot),
		CreateComputer(Race::Terran,sc2::Difficulty::Easy)
	});
	coordinator.SetRealtime(true);
	//coordinator.SetMultithreaded(true);
	coordinator.LaunchStarcraft();
	//while (true) {
		coordinator.StartGame(sc2::kMapBelShirVestigeLE);

		while (coordinator.Update()) {

		}
		std::cout << bot.gameloop << std::endl;
	//}

	return 0;
}
