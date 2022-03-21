clear 
clc
input_raw = cell2mat(table2cell(readtable('foo.csv')));
encoder_raw = readtable('fin.csv');

plot(input_raw(:,1), input_raw(:,2));
yyaxis right
plot(input_raw(:,1), input_raw(:,4));

figure
plot(encoder_raw(:,1), encoder_raw(:,2));

