#ifndef _PATCHSET_
#define _PATCHSET_

class PatchSet {
private: vector<Patch> patches;
		 vector<Ponto> pontos;
public:
	PatchSet(){
		patches = vector<Patch>();
		pontos = vector<Ponto>();
	}
	PatchSet(vector<Patch> p,vector<Ponto> pts){
		this->patches = p;
		this->pontos = pts;
	}
	vector<Patch> getPatches(){
		return this->patches;
	}
	vector<Ponto> getPontos(){
		return this->pontos;
	}
	void setPatches(vector<Patch> p){
		this->patches = p;
	}
	void setPontos(vector<Ponto> p){
		this->pontos = p;
	}
	void addPatch(Patch p){
		this->patches.push_back(p);
	}
	void addPonto(Ponto p){
		this->pontos.push_back(p);
	}
	void preenche(){
		for(int i = 0;patches.size();i++){
			patches[i].preenche(pontos);
		}
	}
	void toString(){
		for(int i = 0;i<this->pontos.size(); i++){
			pontos[i].toString();
		}
	}
	int nrPatches(){
		return this->patches.size();
	}
	int nrPontos(){
		return this->pontos.size();
	}
	

};

#endif