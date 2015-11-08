function [X, val, result]=MIPSolver_v6(A, B, C, contypes, X_Init)

%      maximize    x +   y + 2 z
%      subject to  x + 2 y + 3 z <= 5
%                  x +   y       >= 1
%      x, y, z binary
%
model.modelsense='max';

model.vtype='B';

model.A=A;
model.rhs=B;
model.obj=C;
model.sense=contypes;

if ~isempty(X_Init)
    model.start=X_Init;
end

params.Display = 1;
params.DisplayInterval=1;
params.OutputFlag=1;

params.IterationLimit = 1e5;
params.FeasibilityTol = 1e-6;
%params.IntFeasTol = 1e-5;
params.IntFeasTol = 1e-2;
%params.OptimalityTol = 1e-6;
params.OptimalityTol = 1e-2;
params.Method = 1; % 0 - primal, 1 - dual
params.Presolve = 2; % -1 - auto, 0 - no, 1 - conserv, 2 - aggressive
%params.LogFile = 'MIPSolver.log';
%params.WriteToFile = 'MIPSolver.mps';

result=gurobi(model, params);

X=result.x;
val=result.objval;