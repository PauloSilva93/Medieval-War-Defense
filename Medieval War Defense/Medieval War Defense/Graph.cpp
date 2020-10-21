#include <iostream>
#include "Graph.h"
#include "MathUtils.h"

Node::Node () : vertices () {
	// Inicializando dados
	this ->position[0] = 0;
	this ->position[1] = 0;
	this ->position[2] = 0;
}

std::shared_ptr<Node> Node::NextTarget () {
	if (this -> vertices.size () > 0)
		return this -> vertices [rand() % this -> vertices.size ()];
	else
		return nullptr;
}

Graph::Graph () : mapPath () { // Iniciando Floresta Vazia
	this ->mapPath = nullptr;
}

std::shared_ptr<Node> Graph::genRandInit () {

	if (this->mapPath->vertices.size() > 0) return this->mapPath->vertices[rand() % this->mapPath->vertices.size()];
	else
		return nullptr;
}

void Graph::loadMap1() {
	
	MathUtils mu;
	//Cria fase
	this->mapPath = std::make_shared<Node>();
	// Criando init1
    std::shared_ptr<Node> init1 = std::make_shared<Node>();
    this ->mapPath-> vertices.push_back(init1);
    init1 -> position[0] = mu.GenRandFloat(-1.0, -0.9);
    init1 ->position[1] = mu.GenRandFloat(0.0, 0.2);
	init1->position[2] = ((1.0f - init1->position[1]) / 2.0f);

    // Criando init2
	std::shared_ptr<Node> init2 = std::make_shared<Node>();
    this -> mapPath -> vertices.push_back(init2);
    init2 ->position[0] = mu.GenRandFloat(-0.1, 0.1);
    init2 ->position[1] = mu.GenRandFloat(-1.0, -0.9);
    init2 ->position[2] = ((1.0f - init2->position[1]) / 2.0f);

	// Criando init3
	std::shared_ptr<Node> init3 = std::make_shared<Node>();
	this->mapPath->vertices.push_back(init3);
	init3->position[0] = mu.GenRandFloat(-0.1, 0.1);
	init3->position[1] = mu.GenRandFloat(0.9, 1.0);
	init3->position[2] = ((1.0f - init3->position[1]) / 2.0f);

	// Criando path
	std::shared_ptr<Node> path = std::make_shared<Node>();
	// init1 aponta para path...
    init1 -> vertices.push_back(path);
	// init2 também aponta para path...
	init2->vertices.push_back(path);
	// init3 também aponta para path...
	init3->vertices.push_back(path);
    // agora defino a posicao do Path
    path ->position[0] = mu.GenRandFloat(-0.1, 0.1);
    path ->position[1] = mu.GenRandFloat(0.0, 0.2);
    path ->position[2] = ((1.0f - path->position[1]) / 2.0f);
	// até aqui, os dois inícios apontam para path no centro da tela...

    // Adicionando nós ao path (agora sem nome, e por isso preciso especificar os vértices com path->vertices[n])
	// path aponta para "nó sem nome"
    path -> vertices.push_back(std::make_shared<Node>());
	// como não tem nome, preciso especificar qual o vértice o "nó sem nome está"
    path = path -> vertices[0];
	// agora defino a posição do "nó sem nome"
    path ->position[0] = mu.GenRandFloat(1.1, 1.2);
    path ->position[1] = mu.GenRandFloat(0.0, 0.2);
    path ->position[2] = ((1.0f - path->position[1]) / 2.0f);

}
