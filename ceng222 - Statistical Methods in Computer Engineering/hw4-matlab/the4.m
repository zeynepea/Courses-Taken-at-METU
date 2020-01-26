%%%%%%%%%%%%% Q1 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
N = 38416;
lambda = 35;                    %lambda=7x5
p = 0.2593;
TotalGivenCon = zeros(N,1);
for k=1:N;                      %do loop of N Monte Carlo Runs
        U =rand;
        j=0;
        while U>=exp(-lambda); 
            U=U*rand;
            j = j+1;
        end;
        X = j;                  %creatures caught in 7 hours is generated 
                                %by poisson distrubition X with lamda=35
      
        Y = sum(rand(X,1)<p);   %creatures caught in 7 hours having 
                                %the relationship W >= 2S = 0.2593 is
                                %generated as a sum of Bernoulli(p) variables
        TotalGivenCon(k) = Y;
end;

P_est = mean(TotalGivenCon>8)   %proportion of runs with creatures 
                                %caught in 7 hours having the relationship 
                                %W >= 2S is bigger than 8
                                
%%%%%%%%%%%%% Q2 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
TotalPickle = zeros(N,1);
l = 50;                         %lambda=10x5
for k=1:N;                      
        U =rand;
        j=0;
        while U>=exp(-l); 
            U=U*rand;
            j = j+1;
        end;
        X = j;                                                  
        TotalPickle(k) = X;
end;
TotalWeight = 2 * sum(TotalPickle) / N
%%%%%%%%%%%%% Q3 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Total = zeros(N,1);
for k=1:N;  
    A=exprnd(1/2);
    B=normrnd(0,1);
    Total(k)=(2*A+3*B)/(3+2*abs(B));   
end;
E = sum(Total)/N


