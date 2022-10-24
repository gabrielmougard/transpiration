```

 ______ ______  ______  __   __  ______  ______ __  ______  ______  ______ __  ______  __   __    
/\__  _/\  == \/\  __ \/\ "-.\ \/\  ___\/\  == /\ \/\  == \/\  __ \/\__  _/\ \/\  __ \/\ "-.\ \   
\/_/\ \\ \  __<\ \  __ \ \ \-.  \ \___  \ \  _-\ \ \ \  __<\ \  __ \/_/\ \\ \ \ \ \/\ \ \ \-.  \  
   \ \_\\ \_\ \_\ \_\ \_\ \_\\"\_\/\_____\ \_\  \ \_\ \_\ \_\ \_\ \_\ \ \_\\ \_\ \_____\ \_\\"\_\ 
    \/_/ \/_/ /_/\/_/\/_/\/_/ \/_/\/_____/\/_/   \/_/\/_/ /_/\/_/\/_/  \/_/ \/_/\/_____/\/_/ \/_/ 

"Transpiration" : an encryption transpiler                 
```
(Note: This is an actively developed project so pretty much EVERYTHING is experimental and likely to break...)

**Transpiration** is an end-to-end transpiler for fully homomorphic encryption (FHE) that takes high-level programs and emit an efficient and secure FHE code. The second purpose of this project is also learning the LLVM and the MLIR framework :) If you have no clue of what FHE is, I provide a transcript of a talk I gave not so long ago on my [blog](https://www.norskegab.com/presentation-on-fully-homomorphic-encryption/) (I introduce the base concept of FHE and one of its subset being [TFHE](https://eprint.iacr.org/2018/421.pdf). I created some TFHE experiments [here](https://github.com/gabrielmougard/fhe-experiment)) 

The goal is that it supports Ring-LWE based schemes [B](https://eprint.iacr.org/2012/078)/[FV](https://eprint.iacr.org/2012/144), [BGV](https://eprint.iacr.org/2011/277) and [CKKS](https://eprint.iacr.org/2016/421) which offer powerful [SIMD]-like operations and can _batch_ many thousands of values into a single vector-like ciphertext.

In FHE (and other advanced cryptographic techniques such as MPC or ZKP), developers must express their applications as an (arithmetic/binary) circuit. Translating a function *f* so that the resulting circuit can be evaluated efficiently is highly non-trivial and doing so manually requires significant expert knowledge. This is where FHE compilers like **Transpiration** come in, by automating the transformation of high-level programs into lower-level representations that can be evaluated using FHE.

## Architecture 

**Transpiration** is built using the [MLIR](https://mlir.llvm.org/) compiler framework and follows a traditional front-, middle- and back-end architecture. It uses two Intermediate Representations (IRs) in the middle-end, High-level IR (HIR) to express programs containing control flow and an abstraction of FHE computing (`transpiration::fhe`). 
This is then lowered to Scheme-specific IR (SIR), with operations corresponding to the FHE schemes' underlying operations (e.g., addition, multiplication, relineraization, etc.). Currently, **Transpiration** targets [Microsoft SEAL](https://github.com/Microsoft/SEAL) as its backend (but, I'd like to write a target for [zama.ai's Concrete](https://github.com/zama-ai/concrete) framework). In the future, **Transpiration** will be extended with Polynomial-level IR (PIR) and RNS IR (RIR) to directly target hardware (both CPUs and dedicated FHE accelerators).

## AST representation

The AST consists of nodes that are derived from either `AbstractExpression` or `AbstractStatement`, depending on whether the operation is an expression or a statement, respectively.

```
                                          ┌─────────────────────┐                                                   
                                          │    AbstractNode     │                                                   
                                          └─────────────────────┘                                                   
                                                     ▲                                                              
                                                     │                                                              
                                                     │                                                              
                         ┌─────────────────────┐     │     ┌─────────────────────┐                                  
                         │  AbstractStatement  │─────┴─────│ AbstractExpression  │                                  
                         └─────────────────────┘           └─────────────────────┘                                  
                                    ▲                                 ▲                                             
         ┌─────────────────────┐    │                                 │     ┌─────────────────────┐                 
         │     Assignment      │────┤                                 ├─────│   AbstractTarget    │                 
         └─────────────────────┘    │                                 │     └─────────────────────┘                 
                                    │                                 │                ▲                            
         ┌─────────────────────┐    │                                 │                │     ┌─────────────────────┐
         │        Block        │────┤                                 │                ├─────│  FunctionParameter  │
         └─────────────────────┘    │                                 │                │     └─────────────────────┘
                                    │                                 │                │                            
         ┌─────────────────────┐    │                                 │                │     ┌─────────────────────┐
         │         For         │────┤                                 │                ├─────│     IndexAccess     │
         └─────────────────────┘    │                                 │                │     └─────────────────────┘
                                    │                                 │                │                            
         ┌─────────────────────┐    │                                 │                │     ┌─────────────────────┐
         │      Function       │────┤                                 │                └─────│      Variable       │
         └─────────────────────┘    │                                 │                      └─────────────────────┘
                                    │                                 │                                             
         ┌─────────────────────┐    │                                 │     ┌─────────────────────┐                 
         │         If          │────┤                                 ├─────│  BinaryExpression   │                 
         └─────────────────────┘    │                                 │     └─────────────────────┘                 
                                    │                                 │                                             
         ┌─────────────────────┐    │                                 │     ┌─────────────────────┐                 
         │       Return        │────┤                                 ├─────│ OperatorExpression  │                 
         └─────────────────────┘    │                                 │     └─────────────────────┘                 
                                    │                                 │                                             
         ┌─────────────────────┐    │                                 │     ┌─────────────────────┐                 
         │ VariableDeclaration │────┘                                 ├─────│   UnaryExpression   │                 
         └─────────────────────┘                                      │     └─────────────────────┘                 
                                                                      │                                             
                                                                      │     ┌─────────────────────┐                 
                                                                      ├─────│        Call         │                 
                                                                      │     └─────────────────────┘                 
                                                                      │                                             
                                                                      │     ┌─────────────────────┐                 
                                                                      ├─────│   ExpressionList    │                 
                                                                      │     └─────────────────────┘                 
                                                                      │                                             
                                                                      │     ┌─────────────────────┐                 
                                                                      ├─────│     Literal<T>      │                 
                                                                      │     └─────────────────────┘                 
                                                                      │                                             
                                                                      │     ┌─────────────────────┐                 
                                                                      └─────│   TernaryOperator   │                 
                                                                            └─────────────────────┘                 
```