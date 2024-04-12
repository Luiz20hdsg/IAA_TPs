import sys
import numpy as np
import time
import os
import math
import random


class Env:

    def __init__(self, height, width, posX, posY, epsilon=0.1, alpha=0.1, gamma=0.9):

        self.height = height
        self.width = width
        self.posX = posX
        self.posY = posY
        self.actions = [0, 1, 2, 3]
        self.stateCount = self.height*self.width
        self.actionCount = len(self.actions)
        self.epsilon = epsilon
        self.alpha = alpha
        self.gamma = gamma
        
        


    def reset(self,posX,posY ):
        self.posX = posX
        self.posY = posY
        self.done = False
        return 0, 0, False

    # take action
    def step(self, action, data, id_metodo):

        if action == 0: # left
            if self.posY > 0:

                self.posY = self.posY-1
            else:

                self.posY = self.posY
            #self.posY = self.posY-1 if self.posY > 0 else self.posY
        if action == 1: # right
            if self.posY < self.width - 1:

                self.posY = self.posY+1
            else:
                self.posY = self.posY
            #self.posY = self.posY+1 if self.posY < self.height - 1 else self.posY
        if action == 2: # up
            if self.posX > 0:
                self.posX = self.posX-1
            else:
                self.posX = self.posX
            #self.posX = self.posX-1 if self.posX > 0 else self.posX
        if action == 3: # down
            if self.posX < self.height-1:
                self.posX = self.posX+1
            else:
                self.posX = self.posX
           # self.posX = self.posX+1 if self.posX < self.width - 1 else self.posX
        
        
        if id_metodo == 'standard' or id_metodo == 'stochastic':
            done = data[self.posX][self.posY] == 10.0 or data[self.posX][self.posY] == -10.0
        elif id_metodo == 'positive':
            done = data[self.posX][self.posY] == 10.0 or data[self.posX][self.posY] == 0.0
        # mapping (x,y) position to number between 0 and 5x5-1=24
        nextState =self.height * self.posX + self.posY #self.height * self.posY + self.posX
        reward = data[self.posX][self.posY]
        return nextState, reward, done
    
    def moviment(self, move):
        if move == 0:
            print(">",end="")
        elif move == 1:
            print("<",end="")
        elif move == 2:
            print("^",end="")
        else:
            print("v",end="")

def main():

    if len(sys.argv) < 5:
        print("Número incorreto de argumentos")
        sys.exit() # Encerra a execução do programa

    path_arq = sys.argv[1]
    id_metodo = sys.argv[2]
    xi = int(sys.argv[3])
    yi = int(sys.argv[4])
    n = int(sys.argv[5])
    k = 0
    y = 0

    arquivo = open(path_arq, 'r')
    
    linha = arquivo.readline()
    linha=linha.split()
    

    n_col = int(linha[0])
    n_linhas = int(linha[1])

    data = np.zeros((n_linhas,n_col))
    
    if id_metodo == 'standard' or id_metodo == 'stochastic':
        for linha in arquivo.readlines():

            for i in range(0,len(linha)):

                if linha[i] != ' ' and linha[i] != '\n':
                
                    if linha[i] == '.':
                        data[k][y] = -0.1
                    elif linha[i] == ';':
                        data[k][y] = -0.3
                    elif linha[i] == '+':
                        data[k][y] = -0.1
                    elif linha[i] == 'x':
                        data[k][y] = -10.0
                    elif linha[i] == 'O':
                        data[k][y] = 10
                    else:
                        data[k][y] = -math.inf
                    y = y + 1
            y = 0   
            k = k + 1  

    elif id_metodo == 'positive':
        for linha in arquivo.readlines():

            for i in range(0,len(linha)):

                if linha[i] != ' ' and linha[i] != '\n':
                
                    if linha[i] == '.':
                        data[k][y] = 3.0
                    elif linha[i] == ';':
                        data[k][y] = 1.5
                    elif linha[i] == '+':
                        data[k][y] = 1.0
                    elif linha[i] == 'x':
                        data[k][y] = 0.0
                    elif linha[i] == 'O':
                        data[k][y] = 10
                    else:
                        data[k][y] = math.inf
                    y = y + 1
            y = 0   
            k = k + 1  

       
    
    
    
    # create environment
    env = Env(n_linhas,n_col,xi,yi)
    q_table = np.zeros([env.stateCount, env.actionCount])
    

    # training loop
    for i in range(n):
        state, reward, done = env.reset(xi,yi)
        steps = 0
        r = 0
        while not done:
           
            
            steps += 1
           
            if random.random() < env.epsilon:

                action = random.choice(env.actions) # Explore action space
                

            else:

                action = np.argmax(q_table[state]) # Exploit learned values
                
            
           
            next_state, reward, done = env.step(action,data,id_metodo)

            '''
            qa = max(q_table[next_state,a] for a in range(env.actionCount))
            q_table[state,action] += env.alpha * (reward + env.gamma * qa - q_table[state,action])
            '''
            
            old_value = q_table[state, action]
            next_max = np.max(q_table[next_state])
            
            #new_value = old_value + env.alpha * (reward + env.gamma * next_max - old_value)
            new_value = (1 - env.alpha) * old_value + env.alpha * (reward + env.gamma * next_max)
            q_table[state, action] = new_value
            
            state = next_state
            
            r += reward

       

        #print("\nDone in", steps, "steps".format(steps))
    

    if id_metodo =='stochastic':
        aux = xi*yi
        for row in range(0,n_linhas):
            for col in range(0,n_col):
                if data[row][col] == -10:
                    print("x",end="")
                elif data[row][col] == -math.inf:
                    print("@",end="")
                elif data[row][col] == 10:
                    print("O",end="")
                else:
                    #max_value=max(q_table[row])
                    index = np.argmax(q_table[aux])
                    
                    if index == 0:
                        
                        move = random.choices(env.actions, weights=(80, 0, 10, 10))
                        env.moviment(move[0])

                    elif index == 1:
                        
                        move = random.choices(env.actions, weights=(0, 80, 10, 10))
                        env.moviment(move[0])
                    elif index == 2:
                        
                        move = random.choices(env.actions, weights=(10, 10, 80, 0))
                        env.moviment(move[0])
                    else:
                    
                        move = random.choices(env.actions, weights=(10, 10, 80, 0))
                        env.moviment(move[0])

            print('\n')
    else:
        aux = xi*yi
        for row in range(0,n_linhas):
            for col in range(0,n_col):
                if data[row][col] == -10:
                    print("x",end="")
                elif data[row][col] == -math.inf:
                    print("@",end="")
                elif data[row][col] == 10:
                    print("O",end="")
                else:
                    
                    index = np.argmax(q_table[aux])
                    
                    if index == 0:
                        print(">",end="")
                    elif index == 1:
                        print("<",end="")
                    elif index == 2:
                        print("^",end="")
                    else:
                        print("v",end="")
                aux+=1
            print('\n')
        

    
    
if __name__ == "__main__":
    main()