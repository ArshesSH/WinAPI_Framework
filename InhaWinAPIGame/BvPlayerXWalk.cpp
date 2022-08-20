#include "BvPlayerXWalk.h"

#include "Scene.h"

void PlayerX::WalkStart::Activate( PlayerX& playerX, Scene& scene )
{
    playerX.SetAnimation( PlayerX::AnimationState::WalkStart, walkStartSpeed );
}

PlayerX::Behavior* PlayerX::WalkStart::Update( PlayerX& playerX, Scene& scene, float dt )
{
    if ( playerX.curAnimation.IsEnd() )
    {
        return PassTorch();
    }
    return nullptr;
}

void PlayerX::WalkLoop::Activate( PlayerX& playerX, Scene& scene )
{
    playerX.SetAnimation( PlayerX::AnimationState::WalkLoop, walkLoopSpeed );
}

PlayerX::Behavior* PlayerX::WalkLoop::Update( PlayerX& playerX, Scene& scene, float dt )
{
    if ( HasSucessors() )
    {
        return PassTorch();
    }

    playerX.Move( dt, scene );

    //playerX.Walk( dt, scene );
    return nullptr;
}

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


    return nullptr;
}

void PlayerX::Walk::Move( PlayerX& playerX, float dt )
{
    if ( playerX.isRightKeyDown )
    {
        playerX.vel.x = moveSpeed;
    }
    if ( playerX.isLeftKeyDown )
    {
        playerX.vel.x = -moveSpeed;
    }
    
#ifndef NDBUG
    std::cout << "PlayerSpeedX = " << playerX.vel.x << std::endl;
#endif // !NDBUG


}
