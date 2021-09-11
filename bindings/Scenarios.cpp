//
// Created by per on 9/8/21.
//

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <scenario/scenarios/GeneralAIOneVersusOne.h>
#include <scenario/scenarios/GoldCollectFifteen.h>
#include "scenario/Scenario.h"
#include "scenario/criterias/DamageDone.h"
#include "scenario/criterias/DamageDoneIncrement.h"
#include "scenario/criterias/DamageTaken.h"
#include "scenario/criterias/DamageTakenIncrement.h"
#include "scenario/criterias/FoodConsumption.h"
#include "scenario/criterias/FoodCount.h"
#include "scenario/criterias/GoldCollect.h"
#include "scenario/criterias/LumberCollect.h"
#include "scenario/criterias/NumUnitTypeCreated.h"
#include "scenario/criterias/ResourceIncrement.h"
#include "scenario/criterias/StoneCollect.h"
#include "scenario/criterias/UnitIncrement.h"
#include "scenario/criterias/UnitsCreated.h"
#include "scenario/criterias/GameEnd.h"

#include "trampolines/PyScenarioCriteria.h"

namespace py = pybind11;

// Criterias
using DeepRTS::Criteria::ScenarioCriteria;
using DeepRTS::Criteria::DamageDone;
using DeepRTS::Criteria::DamageDoneIncrement;
using DeepRTS::Criteria::DamageTaken;
using DeepRTS::Criteria::DamageTakenIncrement;
using DeepRTS::Criteria::FoodConsumption;
using DeepRTS::Criteria::FoodCount;
using DeepRTS::Criteria::GoldCollect;
using DeepRTS::Criteria::LumberCollect;
using DeepRTS::Criteria::NumUnitTypeCreated;
using DeepRTS::Criteria::ResourceIncrement;
using DeepRTS::Criteria::StoneCollect;
using DeepRTS::Criteria::UnitIncrement;
using DeepRTS::Criteria::UnitsCreated;
using DeepRTS::Criteria::GameEnd;

using namespace DeepRTS;

// Scenarios
using DeepRTS::Scenarios::GeneralAIOneVersusOne;
using DeepRTS::Scenarios::GoldCollectFifteen;
using DeepRTS::Scenarios::Scenario;

using DeepRTS::Config;
using DeepRTS::Game;


void init_scenarios(py::module &m) {
    auto scenario_module = m.def_submodule("scenario");
    auto criteria_module = scenario_module.def_submodule("criteria");

    py::class_<Scenario, Game>(scenario_module, "Scenario")
            .def(py::init<const std::string&, Config, DeepRTS::Criteria::ScenarioCriteria::ScenarioContainer>())
            .def("update", &Scenario::update)
            .def("evaluate", &Scenario::evaluate)
            .def("reward", &Scenario::reward)
            .def("optimal_strategy", &Scenario::optimalStrategy)
            .def("compute_optimal_strategy", &Scenario::computeOptimalStrategy)
            .def("optimal_play_game_step", &Scenario::optimalPlayGameStep);

    py::class_<GoldCollectFifteen, Scenario>(scenario_module, "GoldCollectFifteen")
            .def(py::init<>())
            .def("update", &Scenario::update)
            .def("evaluate", &Scenario::evaluate)
            .def("reward", &Scenario::reward)
            .def("optimal_strategy", &Scenario::optimalStrategy)
            .def("compute_optimal_strategy", &Scenario::computeOptimalStrategy)
            .def("optimal_play_game_step", &Scenario::optimalPlayGameStep);

    py::class_<GeneralAIOneVersusOne, Scenario>(scenario_module, "GeneralAIOneVersusOne")
            .def(py::init<>())
            .def("update", &Scenario::update)
            .def("evaluate", &Scenario::evaluate)
            .def("reward", &Scenario::reward)
            .def("optimal_strategy", &Scenario::optimalStrategy)
            .def("compute_optimal_strategy", &Scenario::computeOptimalStrategy)
            .def("optimal_play_game_step", &Scenario::optimalPlayGameStep);

#define ScenarioFunctionsMacro() \
            .def("clone", &ScenarioCriteria::clone)\
            .def("evaluate", &ScenarioCriteria::evaluate)\
            .def("reward", &ScenarioCriteria::reward);

    py::class_<ScenarioCriteria, PyScenarioCriteria, std::shared_ptr<ScenarioCriteria>>(criteria_module, "ScenarioCriteria")
            .def(py::init<int, int>())
            ScenarioFunctionsMacro()


    py::class_<DamageDone, ScenarioCriteria, std::shared_ptr<DamageDone>>(criteria_module, "DamageDone")
            .def(py::init<int, int>())
            ScenarioFunctionsMacro()

    py::class_<DamageDoneIncrement, ScenarioCriteria, std::shared_ptr<DamageDoneIncrement>>(criteria_module, "DamageDoneIncrement")
            .def(py::init<int, int>())
            ScenarioFunctionsMacro()

    py::class_<DamageTaken, ScenarioCriteria, std::shared_ptr<DamageTaken>>(criteria_module, "DamageTaken")
            .def(py::init<int, int>())
            ScenarioFunctionsMacro()

    py::class_<DamageTakenIncrement, ScenarioCriteria, std::shared_ptr<DamageTakenIncrement>>(criteria_module, "DamageTakenIncrement")
            .def(py::init<int, int>())
            ScenarioFunctionsMacro()

    py::class_<FoodConsumption, ScenarioCriteria, std::shared_ptr<FoodConsumption>>(criteria_module, "FoodConsumption")
            .def(py::init<int, int>())
            ScenarioFunctionsMacro()

    py::class_<FoodCount, ScenarioCriteria, std::shared_ptr<FoodCount>>(criteria_module, "FoodCount")
            .def(py::init<int, int>())
            ScenarioFunctionsMacro()

    py::class_<GoldCollect, ScenarioCriteria, std::shared_ptr<GoldCollect>>(criteria_module, "GoldCollect")
            .def(py::init<int, int>())
            ScenarioFunctionsMacro()

    py::class_<LumberCollect, ScenarioCriteria, std::shared_ptr<LumberCollect>>(criteria_module, "LumberCollect")
            .def(py::init<int, int>())
         ScenarioFunctionsMacro()

    py::class_<NumUnitTypeCreated, ScenarioCriteria, std::shared_ptr<NumUnitTypeCreated>>(criteria_module, "NumUnitTypeCreated")
            .def(py::init<Constants::Unit, int, int, int>())
            ScenarioFunctionsMacro()

    py::class_<ResourceIncrement, ScenarioCriteria, std::shared_ptr<ResourceIncrement>>(criteria_module, "ResourceIncrement")
            .def(py::init<Constants::Resource, int, int, int>())
         ScenarioFunctionsMacro()

    py::class_<StoneCollect, ScenarioCriteria, std::shared_ptr<StoneCollect>>(criteria_module, "StoneCollect")
            .def(py::init<int, int>())
            ScenarioFunctionsMacro()

    py::class_<UnitIncrement, ScenarioCriteria, std::shared_ptr<UnitIncrement>>(criteria_module, "UnitIncrement")
            .def(py::init<Constants::Unit, int, int, int>())
            ScenarioFunctionsMacro()

    py::class_<UnitsCreated, ScenarioCriteria, std::shared_ptr<UnitsCreated>>(criteria_module, "UnitsCreated")
            .def(py::init<int, int>())
            ScenarioFunctionsMacro()

    py::class_<GameEnd, ScenarioCriteria, std::shared_ptr<GameEnd>>(criteria_module, "GameEnd")
            .def(py::init<int, int>())
            ScenarioFunctionsMacro()

}