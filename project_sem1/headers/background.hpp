int bgIndex = 0;
int prevIndex = 0;
float bgTimer = 0.f;
float bgFrameTime = 1.f / 30.f;

void updateBackground(float dt)
{
    bgTimer += dt;
    prevIndex = bgIndex;
    bgIndex = (int)bgTimer % 120;
}


