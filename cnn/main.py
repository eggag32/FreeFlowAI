import os
import torch
from torch import nn
from torchvision.transforms import ToTensor
from torch.utils.data import DataLoader, Dataset
import matplotlib.pyplot as plt
import CNN

class gridData(Dataset):
    def __init__(self, file):
        self.data = self.load_data(file)

    def load_data(self, file):
        data = []
        with open(file, 'r') as f:
            cnt = 0
            pre = torch.zeros(7, 6, 6)
            for line in f:
                l = list(map(float, line.strip().split()))
                if cnt % 2 == 1:
                    tensor = torch.tensor(l, dtype=torch.int64).reshape(6, 6)
                    data.append((pre, tensor))
                else:
                    pre = torch.tensor(l, dtype=torch.float32).reshape(7, 6, 6)
                cnt += 1
        return data

    def __len__(self):
        return len(self.data)

    def __getitem__(self, idx):
        return self.data[idx]

training_data = gridData('6x6.txt')
train_dataloader = DataLoader(training_data, batch_size=64)

test_data = gridData('test_6x6.txt')
test_dataloader = DataLoader(test_data, batch_size=64)

device = ("cuda" if torch.cuda.is_available() else "cpu")
print(f"Using {device} device")

model_path = 'flow_free_model.pth'
if os.path.exists(model_path):
    model = torch.load(model_path)
else:
    model = CNN.CNN().to(device)
print(model)

def train_loop(dataloader, model, loss_fn, optimizer):
    size = len(dataloader.dataset)
    model.train()
    for batch, (X, y) in enumerate(dataloader):
        X = X.to(device)
        y = y.to(device)
        pred = model(X)
        loss = loss_fn(pred, y)

        loss.backward()
        optimizer.step()
        optimizer.zero_grad()

        if batch % 1000 == 0:
            loss, current = loss.item(), batch * batch_size + len(X)
            print(f"loss: {loss:>7f} [{current:>5d}|{size:5d}]")

def test_loop(dataloader, model, loss_fn):
    model.eval()
    size = len(dataloader.dataset)
    num_batches = len(dataloader)
    test_loss, correct = 0, 0
    tot = 0

    with torch.no_grad():
        for X, y in dataloader:
            X = X.to(device)
            y = y.to(device)
            pred = model(X)
            test_loss += loss_fn(pred, y).item()
            pred_classes = pred.argmax(1)
            correct += (pred_classes == y).all(dim=(1, 2)).sum().item()
            tot += (pred_classes == y).all(dim=(1, 2)).sum().item()

        test_loss /= num_batches
        correct /= size
        print(f"Test Error: \n Accuracy: {(100*correct):>0.1f}%, Avg loss: {test_loss:>8f} \n")
        print(f"Number correct: {tot}")

learning_rate = 1e-2
batch_size = 64
epochs = 200

loss_fn = nn.CrossEntropyLoss()
optimizer = torch.optim.SGD(model.parameters(), lr=learning_rate)

for t in range(epochs):
    print(f"Epoch {t+1}\n-------------------------------")
    train_loop(train_dataloader, model, loss_fn, optimizer)
    test_loop(test_dataloader, model, loss_fn)
    torch.save(model, model_path)

print("Done!")