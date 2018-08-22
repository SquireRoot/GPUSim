model = readModelInfo('GPUSim.config');
model = readPoints('points.data', model);
write2DGiff(model, 750, 750, 'data.giff');