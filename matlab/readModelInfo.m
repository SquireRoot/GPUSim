function [out] = readModelInfo(filename)

fh = fopen(filename);
file = textscan(fh, '%s %s %d', 4);
data = file{3};

out.sizeX = data(1);
out.sizeY = data(2);
out.sizeZ = data(3);
out.iterations = data(4);

file = textscan(fh, '%s %s %f', 4);
data = file{3};
out.timeStep = data(1);
out.eqDistance = data(2);
out.springConst = data(3);
out.mass = data(4);

out.time = (0:out.iterations) .* out.timeStep;

end