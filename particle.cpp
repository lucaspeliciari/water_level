class Particle
{
    public:
        int x = 0;
        int y = 0;
        int volume = 1000;  // equivalent to 1000 liters

    Particle(int nx, int ny)
    {
        x = nx;
        y = ny;
    }

    void Update(int g)
    {
        y += g;
    }
};
