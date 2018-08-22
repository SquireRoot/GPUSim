function [] = write2DGiff(model, xres, yres, filename)

fig = figure('position', [50, 50, xres, yres]);
hold off
for i = 1:model.iterations
    clf(fig);
    x = reshape(model.points(i, :, :, 1, 1), [model.sizeX.*model.sizeY, 1]);
    y = reshape(model.points(i, :, :, 1, 2), [model.sizeX.*model.sizeY, 1]);
    plot(x, y, 'or');
    axis([-1 50 -1 50]);
    drawnow;
    
    frame = getframe(fig);
    img = frame2im(frame);
    [A, map] = rgb2ind(img, 256);
    
    if i == 1
        imwrite(A,map,filename,'gif','LoopCount',Inf,'DelayTime',model.timeStep);
    else
        imwrite(A,map,filename,'gif','WriteMode','append','DelayTime',model.timeStep);
    end
end

end