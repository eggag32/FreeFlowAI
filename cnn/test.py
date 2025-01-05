import os
import torch
import CNN
from torch import nn
from CNN import CNN

model_path = 'flow_free_model.pth'
if os.path.exists(model_path):
    model = torch.load(model_path, map_location=torch.device('cpu'))
    model.eval()
else:
    print("Model not found")
    exit(0)
print(model)

print("Give 6x6 board as input (blank squares 0, colors indexed starting from 1):")
g = []
for i in range(6):
    g.append(list(map(int, input().split())))

inp = torch.zeros(7, 6, 6)

mx = max(max(row) for row in g)
m = [-1 for _ in range(mx)]
ind = 0
for i in range(6):
    for j in range(6):
        if g[i][j] > 0 and m[g[i][j] - 1] == -1:
            m[g[i][j] - 1] = ind 
            ind += 1

for i in range(6):
    for j in range(6):
        if g[i][j] != 0:
            inp[m[g[i][j] - 1]][i][j] = 1

inp = inp.unsqueeze(0)
pred = model(inp)
pred_classes = pred.argmax(1)
print("\nPredicted Grid:")
print(pred_classes[0].cpu().numpy())