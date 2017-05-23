#ifndef _PREDICTOR_H_
#define _PREDICTOR_H_

#include "utils.h"
#include "tracer.h"
#include "math.h"

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

class PREDICTOR{

 private:
  int  *ghr;           // global history register
  UINT32  inputLength;
  UINT32  outputLength;
  UINT32  theta;
  int **W;
  int *sum; //the result of sum before threshold

 public:

  // The interface to the four functions below CAN NOT be changed

  PREDICTOR(void);
  ~PREDICTOR(void);
  bool    GetPrediction(UINT32 PC);  
  void    UpdatePredictor(UINT32 PC, bool resolveDir, bool predDir, UINT32 branchTarget);
  void    TrackOtherInst(UINT32 PC, OpType opType, UINT32 branchTarget);

  // Contestants can define their own functions below

};

#endif

