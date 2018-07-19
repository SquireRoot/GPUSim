
file = fopen('../out.pts');
data = textscan(file, '%f %f %f %f %f %f %f %f %f', 1000);
x1 = [data{1:end, 1}];
y1 = [data{1:end, 2}];
z1 = [data{1:end, 3}];

x2 = [data{1:end, 4}];
y2 = [data{1:end, 5}];
z2 = [data{1:end, 6}];

x3 = [data{1:end, 7}];
y3 = [data{1:end, 8}];
z3 = [data{1:end, 9}];

figure(1);
for i = 1:length(x1)
    plot([x1(i) x2(i) x3(i)], [y1(i), y2(i), y3(i)]);
    axis([-0.5 2.5 -0.5 1.5]);
    drawnow;
end