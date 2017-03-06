#pragma once


#include <pybind11/pybind11.h>
#include <pybind11/eval.h>
#include <pybind11/stl.h>
#include <iostream>

class Game;
class CRemoteAI {
public:
	int running = false;
	int meaningoflife = 42;
	pybind11::object pyInstance;
	int gameID;


	void play();
	std::string getState(int gameID);


	static void PySetupImport();
	static void PyRunImport();

	CRemoteAI(int gameID);
	CRemoteAI::CRemoteAI(pybind11::object pyInstance, int gameID);
	static CRemoteAI CRemoteAI::createInstance(int gameID);


	

};