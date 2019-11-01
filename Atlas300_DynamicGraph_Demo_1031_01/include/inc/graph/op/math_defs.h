#ifndef _CCE_GRAPH_OP_GE_OP_MATH_DEFS_H
#define _CCE_GRAPH_OP_GE_OP_MATH_DEFS_H

#include "../operator_reg.h"

namespace ge {
/**
 * Performs tensor addition.
 * <Input>
 *      x1 : First operand.
 *      x2 : Second operand.
 *           1. The input shape can be 1-4 dimensions, less than 4 dimensions, and processed into 4 dimensions;
 *                    1D, will add 1 in the front and 2 in the back, eg:5 After processing: 1,5,1,1
 *                    2D, add 1 to the front and back, eg:5,5 After processing: 1,5,5,1
 *                    3D, add 1 in front, eg:5,5,5, after processing: 1,5,5,5
 *           2. The corresponding n, c, h, w must be equal or equal to 1
 *           3. The input data n, c, h, w must correspond to n, c, h, w greater than or equal to weight
 * <Output>
 *      y  : Result, has same element type as two inputs.
 */
REG_OP(Add)
.INPUT(x1, TensorType({ DT_FLOAT }))
.INPUT(x2, TensorType({ DT_FLOAT })) 
.OUTPUT(y, TensorType({ DT_FLOAT }))
.OP_END()

/**
 * Performs tensor multiplication.
 * <Input>
 *      x : First operand.
 *      y : Second operand.
 *          when x.Format == y.Format == CC_TENSOR_NC1HWC0, the Input of x,y only support:
 *              1.  x:(xN, xC, xH, xW)          y:(1, 1, 1, 1)
 *              2.  x:(xN, xC, xH, xW)          y:(yN=xN, yC=xC, yH=xH, yW=xW)
 *              3.  x:(xN, xC, xH, xW)          y:(1, yC=xC, 1, 1)
 *              4.  x:(1, 1, 1, 1)              y:(yN, yC, yH, yW)
 *              5.  x:(1, xC, 1, 1)             y:(yN, yC=xC, yH, yW)
 *              6.  x:(xN, xC, 1, 1)            y:(yN=xN, yC=xC, yH, yW)
 *              7.  x:(xN, xC, xH, xW)          y:(yN=xN, yC=xC, 1, 1)
 *              8.  x:(1, xC, xH, 1)            y:(yN, yC=xC, yH=xH, yW)
 *              9.  x:(xN, xC, xH, xW)          y:(1, yC=xC, yH=xH, 1)
 *              10.  x:(1, xC, xH, xW)          y:(yN, yC=xC, yH=xH, yW=xW)
 *              11.  x:(xN, xC, xH, xW)         y:(1, yC=xC, yH=xH, yW=xW)
 *              12.  x:(xN, [1,16], xH, xW)     y:(yN=xN, yC, yH=xH, yW=xW)
 *              13.  x:(xN, xC, xH, xW)         y:(yN=xN, [1,16], yH=xH, yW=xW)
 *              14.  x:(xN, xC, 1, xW)          y:(yN=xN, yC=xC, yH, yW=xW)
 *              15.  x:(xN, xC, xH, xW)         y:(yN=xN, yC=xC, 1, yW=xW)
 * <Output>
 *      z : Result, has same element type as two inputs.
 */
REG_OP(Mul)
.INPUT(x, TensorType({ DT_FLOAT }))
.INPUT(y, TensorType({ DT_FLOAT }))
.OUTPUT(z, TensorType({DT_FLOAT}))
.OP_END()

}  // namespace ge

#endif  // _CCE_GRAPH_OP_GE_OP_MATH_DEFS_H
