#include "BvPlayerXWalk.h"
#include "Scene.h"

void PlayerX::Walk::Activate( PlayerX& playerX, Scene& scene )
{
    playerX.SetAnimation( PlayerX::AnimationState::WalkStart, animStartSpeed );
}

PlayerX::Behavior* PlayerX::Walk::Update( PlayerX& playerX, Scene& scene, float dt )
{
    if ( HasSucessors() )
    {
        return PassTorch();
    }

    if ( playerX.curAnimState == AnimationState::WalkStart && playerX.curAnimation.IsEnd() )
    {
        playerX.SetAnimation( AnimationState::WalkLoop, animLoopSpeed );
    }
    Move( playerX, scene, dt );

    return nullptr;
}

void PlayerX::Walk::Move( PlayerX& playerX, Scene& scene, float dt )
{
    if ( playerX.isRightKeyDown )
    {
        playerX.vel.x = moveSpeed;
    }
    if ( playerX.isLeftKeyDown )
    {
        playerX.vel.x = -moveSpeed;
    }
    playerX.Move( dt, scene );
    
#ifndef NDBUG
    std::cout << "PlayerSpeedX = " << playerX.vel.x << std::endl;
#endif // !NDBUG


}
