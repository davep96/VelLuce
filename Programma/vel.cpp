// Program for data analysis of the "measuring the speed of light"
// experiment
// Università degli Studi di Milano, Fisica:
// Laboratorio di Ottica, Elettronica e Fisica moderna
// Author: Davide Passaro
// Start: 10/23/2016
#include <iostream>
#include <fstream> //ifstream ofstream
#include <cmath> // pow()...

struct pair{
	pair()
	{
		value=0;
		zero=0;
	}
	~pair()
	{
	}
	
	double value;
	double zero;
};
struct measure{
	measure()
	{
		value=0;
		error=0;
	}
	~measure()
	{
	}
	
	double value;
	double error;
};

measure wave(measure* values, int num)
{
	measure ave;
	double denom=0;
	for(int i=0; i<num; i++)
	{
		ave.value+=values[i].value*pow(values[i].error,-2);
		denom+=pow(values[i].error,-2);
	}
	
	ave.value=ave.value/denom;
	ave.error=pow(denom,-0.5);
	return ave;
}


struct data{
	public:
	
	data(int num = 100) //Constructor
	{
		f2=0;
		D=0;
		a=0;
		n=num;
		valueNum=0;
		ang=new pair[n];
		dMic=new pair[n];
	}

	~data() //Destructor
	{
		if(n>0)
		{
			delete[] ang;
			delete[] dMic;
		}
	}
	
	void loadData(std::ifstream &);
	void deriveC();
	void printC(measure *);
	
	private:

	int n;
	int valueNum;
	pair *ang;
	double angError;
	pair *dMic;
	double dMicError;
	measure c;
	double f2;
	double f2Error;
	double D;
	double DError;
	double a;
	double aError;
	
};

void
data::loadData(std::ifstream & inFile)
{
	double tester;
	std::string unused;
	
	inFile>>tester;
	f2=tester;
	inFile>>tester;
	f2Error=tester;
	
	inFile>>tester;
	D=tester;
	inFile>>tester;
	DError=tester;
	
	inFile>>tester;
	a=tester;
	inFile>>tester;
	aError=tester;
	
	while(inFile.good())
	{
		inFile>>ang[valueNum].value;
		inFile>>ang[valueNum].zero;
		inFile>>dMic[valueNum].value;
		inFile>>dMic[valueNum].zero;
		valueNum++;
	}

	return;
}

void 
data::deriveC()
{
	measure* expC;
	expC=new measure[valueNum];
	
	for(int i=0; i<valueNum; i++)
	{
		expC[i].value=4*f2*pow(D,2)*(ang[i].value-ang[i].zero)/
				((D+a-f2)*(dMic[i].value-dMic[i].zero));
		expC[i].error=sqrt(
					  pow(
					  f2Error*
					  (4*pow(D,2)*(ang[i].value-ang[i].zero)/
					  (dMic->value-dMic->zero))*(D+a-2*f2)/pow((D+a-f2),2)
					  ,2)+
					  pow(
					  aError*
					  4*f2*pow(D,2)*(ang[i].value-ang[i].zero)/
					  (pow((D+a-f2),2)*(dMic[i].value-dMic[i].zero))
					  ,2)+
					  pow(
					  DError*
					  (4*f2*(ang[i].value-ang[i].zero)/
					  (dMic[i].value-dMic[i].zero))*
					  (2*D*(D+a-f2)-pow(D,2))/pow((D+a-f2),2)
					  ,2)
					  );
	}
	
	printC(expC);
	
	c=wave(expC,valueNum);
	delete[] expC;
}

void 
data::printC(measure* expC )
{
	std::ofstream out;
	out.open("ExpLightSpeeds.txt");
	out<<"Exp Light Speeds \t Errors \t Relative Errors"<<std::endl;
	for(int i=0; i<valueNum; i++)
	{
		std::cout<<i<<std::endl;
		out<<expC[i].value<<"\t"<<expC[i].error<<"\t"
		<<expC[i].error/expC[i].value<<std::endl;
	}
	
	out<<"Light Speed \t Error \t Relative Error"<<std::endl;
	out<<wave(expC,valueNum).value<<"\t"<<wave(expC,valueNum).error<<"\t"
	<<wave(expC,valueNum).error/wave(expC,valueNum).value<<std::endl;
}

int main(){
	
	data dataValues;
	std::ifstream in;
	in.open("data.txt");
	dataValues.loadData(in);
	dataValues.deriveC();
	
	return 0;
}