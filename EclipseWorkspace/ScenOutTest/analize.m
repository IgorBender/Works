function analize(filename)
matr = load(filename);
[r,c] = size(matr);
for i = 1:r
    if matr(i,2) == 0
        for j = 4:c
            matr(i,j) = NaN;
        end
    end
end
subplot(2, 2, 1)
plot(matr(:,1), matr(:,4), '.', 'MarkerSize', 8), grid on, grid minor;
title('Azimuth');
xlabel('Frame')
ylabel('Azimuth')
subplot(2, 2, 2)
plot(matr(:,1), matr(:,5), '.', 'MarkerSize', 8), grid on, grid minor;
title('Elevation');
xlabel('Frame')
ylabel('Elevation')
subplot(2, 2, 3)
plot(matr(:,4), matr(:,5), '.', 'MarkerSize', 8), grid on, grid minor;
title('Trajectory');
xlabel('Azimuth')
ylabel('Elevation')
subplot(2, 2, 4)
plot3(matr(:,1), matr(:,4), matr(:,5), '.', 'MarkerSize', 8), grid on, grid minor;
title('3D');
xlabel('Frame')
ylabel('Azimuth')
zlabel('Elevation')
%view(-10, 45)
end