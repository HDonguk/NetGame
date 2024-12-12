#pragma once
#include <vector>
#include <queue>
#include <winsock2.h> // �ùٸ� ��ġ�� �̵�
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Event.h"
#include "Packet.h"
#include "Utility.h"
#include "Obstacle.h" 
#include <chrono>
#include"stdafx.h"

class GameThread {
public:
    GameThread(); // �⺻ ������ �߰�
    GameThread(SOCKET serverSocket);
    ~GameThread();

    void run();  // ���� ������ ����
    void stop(); // ���� ������ ����

    void updateBulletStatus();
    void updatePlayerStatus();
    bool CheckCollision(const Player& player);

    // ���� ��ü ���
    std::vector<Player> players;
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;
    std::vector<Obstacle*> obstacles;
private:
    
    void waitUntilNextFrame(const std::chrono::time_point<std::chrono::steady_clock>& frameStartTime); // ������ �� ����ȭ

    bool gameRunning; // ���� ���� ���� ����
    std::queue<Event> eventQueue; // �̺�Ʈ ť

   

    // ��Ŷ ����
    vector<PlayerStatusPacket> makeSendPlayerPacket();

    // ���� ��� ���� ����
    SOCKET serverSocket; // ������ �κ�
};
