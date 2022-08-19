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
    playerX.Walk( dt, scene );
    return nullptr;
}
