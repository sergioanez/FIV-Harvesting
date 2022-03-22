clear 
clc
close all
w = 0.1;
encoder_raw2 = [];
t_shift = 1.6;
input_raw = cell2mat(table2cell(readtable('input_57_3waves_a=5_w=2.csv')));
encoder_raw = cell2mat(table2cell(readtable('encoder_57.csv',detectImportOptions('fin.csv'))));

plot(input_raw(:,1), input_raw(:,2));
yyaxis right
plot(input_raw(:,1), input_raw(:,4));

% figure
% plot(encoder_raw(:,1), encoder_raw(:,1));

figure 
plot(encoder_raw(:,1), encoder_raw(:,4), input_raw(:,1), input_raw(:,2));

legend({'Enc','In'})
% 
% figure
% plot(encoder_raw(:,1), encoder_raw(:,2)/100000);