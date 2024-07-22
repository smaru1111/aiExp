import csv
import random
import math

def generate_mean_reward_data(num_episodes, mode):
    rewards = []
    for episode in range(1, num_episodes + 1):
        if mode == "reinforcement":
            # 強化学習の報酬が徐々に増加
            base_reward = 1.2 * math.exp(0.1 * episode)
            random_noise = random.uniform(0.05, 0.15)
        elif mode == "imitation":
            # 模倣学習の報酬が急激に増加
            base_reward = 2.0 * math.exp(0.3 * episode)
            random_noise = random.uniform(0.1, 0.3)
        
        reward = base_reward + random_noise
        rewards.append(reward)
    return rewards

def write_csv(file_name, data):
    with open(file_name, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(['Episode', 'Mean Reward'])
        for i, reward in enumerate(data):
            writer.writerow([i*12000, reward])

# 強化学習のMean Rewardデータ生成
num_episodes_reinforcement = 10
reinforcement_rewards = generate_mean_reward_data(num_episodes_reinforcement, "reinforcement")
write_csv('mean_reward_reinforcement.csv', reinforcement_rewards)

# 模倣学習のMean Rewardデータ生成
num_episodes_imitation = 20
imitation_rewards = generate_mean_reward_data(num_episodes_imitation, "imitation")
write_csv('mean_reward_imitation.csv', imitation_rewards)

print("CSVファイルの生成が完了しました。")
