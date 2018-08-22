function [model] = readPoints( filename, model )

fh = fopen(filename);
model.out = zeros(model.iterations, model.sizeY, model.sizeX, model.sizeZ, 3);
for i = 1:model.iterations
    for z = 1:model.sizeZ
        for y = 1:model.sizeY
            for x = 1:model.sizeX
                a = fread(fh, 3, 'float32');
                if isempty(a)
                    return;
                end
                model.points(i, y, x, z, :) = a;
            end
        end
    end
end

end

