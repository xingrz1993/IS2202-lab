//Renzhi Xing
//931011-2025

#include "predictor.h"
// the predictor is based on preceptron learning neural network， and paper "Neural Methods for Dynamic Branch Prediction"

//Num of input nodes
//which is the length of history, best performing between 4 and 66
#define IN_LEN	35

//Num of output nodes
//which is also the number of perceptrons of the table
#define OUT_LEN	250

//Weight Size
//According to the paper, 8-bit is a good choice
#define WEIGHT_SIZE_MAX 128
#define WEIGHT_SIZE_MIN -127


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

PREDICTOR::PREDICTOR(void){

  inputLength = IN_LEN;
  outputLength = OUT_LEN;
  theta = floor(1.93 * (float)IN_LEN + 14);
  
  // Defination of Weight Matrix
  W = new int*[outputLength];
  for (int i = 0; i < outputLength; i++) 
	  W[i] = new int[inputLength + 1]; 
  // Arrays storing the history
  ghr = new int[inputLength + 1];
  sum = new int[outputLength];
  
  //initialize W and ghr
  ghr[0] = 1; //this is for bias, will always be 1
  for (int j = 1; j <= inputLength; j++){
	  ghr[j] = -1;
  }
  for (int i = 0; i < outputLength; i++){
	  sum[i] = 0;
	  for (int j = 0; j <= inputLength; j++){
		  W[i][j] = 0;
	  }
  }
  //end initialization
  
}

PREDICTOR::~PREDICTOR(void){
	for (int i = 0; i < outputLength; i++)
		delete[] W[i];
	delete[] W;
	delete[] ghr;
	delete[] sum;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

bool  PREDICTOR::GetPrediction(UINT32 PC){
//***** 1. The branch address is hashed to produce an index 0~IN_LEN-1 into the table of perceptrons.
  UINT32 index = PC % outputLength;
  sum[index] = 0;  
//***** 2. fetch the perceptron
//***** 3. calculate value of y
  for(int i = 0; i < IN_LEN ; i++){
	  sum[index] += W[index][i] * ghr[i];
  }  
//***** 4. if y is negative, branch is not taken, otherwise taken
  return ((sum[index] < 0)?NOT_TAKEN:TAKEN);
}


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void  PREDICTOR::UpdatePredictor(UINT32 PC, bool resolveDir, bool predDir, UINT32 branchTarget){
  UINT32 index = PC % (outputLength);
// Update of Weight Matrix  
// 5. updating vector register
// 6. write back to the vector register
  if (predDir != resolveDir || sum[index] <= theta){ 
	  for(int i = 0; i <= inputLength; i++){
		  if(((ghr[i]+1)/2) == resolveDir){
			  if(W[index][i] < 127) //overflow checking
				  W[index][i]++; 
		  }
		  else{
			  if(W[index][i] > -128)
				  W[index][i]--;
		  }
	  }
  }

// update the GHR
  for(int i = 1; i < inputLength; i++){
	  ghr[i] = ghr[i+1];
  }

  if(resolveDir == TAKEN){
      ghr[inputLength] = 1; 
  }else{
	  ghr[inputLength] = -1;
  }
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void    PREDICTOR::TrackOtherInst(UINT32 PC, OpType opType, UINT32 branchTarget){

  // This function is called for instructions which are not
  // conditional branches, just in case someone decides to design
  // a predictor that uses information from such instructions.
  // We expect most contestants to leave this function untouched.

  return;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
