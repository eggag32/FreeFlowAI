import torch
from torch import nn

class CNN(nn.Module):
    def __init__(self):
        super().__init__()
        self.flatten = nn.Flatten()
        self.cnn = nn.Sequential(
            nn.Conv2d(6, 32, kernel_size=5, stride=1, padding=2),
            nn.ReLU(),
            nn.Conv2d(32, 64, kernel_size=3, stride=1, padding=1),
            nn.ReLU(),
            nn.Conv2d(64, 128, kernel_size=3, stride=1, padding=1),
            nn.ReLU()
        )
        self.fc_input_features = 128 * 6 * 6
        self.fc = nn.Sequential(
            nn.Linear(self.fc_input_features, 2048),
            nn.ReLU(),
            nn.Linear(2048, 2048),
            nn.ReLU(),
            nn.Linear(2048, 7 * 6 * 6)
        )

    def forward(self, x):
        x = self.cnn(x)
        x = self.flatten(x)
        logits = self.fc(x)
        logits = logits.view(-1, 7, 6, 6)
        return logits