clear 
clc
close all
w = 0.1;
t_shift = 0.25*(1/w);
input_raw = cell2mat(table2cell(readtable('foo.csv')));
encoder_raw = cell2mat(table2cell(readtable('fin.csv')));

plot(input_raw(:,1), input_raw(:,3));
yyaxis right
plot(input_raw(:,1), input_raw(:,4));

figure
plot(encoder_raw(:,1), encoder_raw(:,5));

figure 
plot(encoder_raw(:,1), encoder_raw(:,5), input_raw(:,1), input_raw(:,3));

legend({'Enc','In'})

figure
plot(encoder_raw(:,1), encoder_raw(:,2)/100000);