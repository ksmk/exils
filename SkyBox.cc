#include "SkyBox.hh"

SkyBox::SkyBox(){}
SkyBox::~SkyBox(){}

void SkyBox::Load(Float zfar)
{
    Float dst = zfar * cos(45.0f);
    Float th  =  1.0f / 3.0f;
    Float of  = (1.0f / 1024.0f) / 2;
    SkyBox_id = glGenLists(1);

    glNewList(SkyBox_id, GL_COMPILE);

    glBegin(GL_QUADS);
        glTexCoord2f(0.25f +of, th   -of);  glVertex3f(  dst,  dst,  dst );
        glTexCoord2f(0.50f -of, th   -of);  glVertex3f( -dst,  dst,  dst );
        glTexCoord2f(0.50f -of, 0.0f +of);  glVertex3f( -dst,  dst, -dst );
        glTexCoord2f(0.25f -of, 0.0f +of);  glVertex3f( -dst,  dst, -dst );
    glEnd();

    glBegin(GL_QUADS);
        glTexCoord2f(0.25f +of, 1.0f -of);  glVertex3f(  dst, -dst, -dst );
        glTexCoord2f(0.50f -of, 1.0f -of);  glVertex3f( -dst, -dst, -dst );
        glTexCoord2f(0.50f -of, 2*th +of);  glVertex3f( -dst,  dst,  dst );
        glTexCoord2f(0.25f -of, 2*th +of);  glVertex3f(  dst, -dst,  dst );
    glEnd();

    glBegin(GL_QUADS);
        glTexCoord2f(0.25f    , 2*th -of);  glVertex3f(  dst, -dst,  dst );
        glTexCoord2f(0.50f    , 2*th -of);  glVertex3f( -dst, -dst,  dst );
        glTexCoord2f(0.50f    , 1*th +of);  glVertex3f( -dst,  dst,  dst );
        glTexCoord2f(0.25f    , 1*th +of);  glVertex3f(  dst,  dst,  dst );
    glEnd();

    glBegin(GL_QUADS);
        glTexCoord2f(0.75f    , 2*th -of);  glVertex3f( -dst, -dst,  dst );
        glTexCoord2f(1.00f    , 2*th -of);  glVertex3f( -dst, -dst, -dst );
        glTexCoord2f(1.00f    , 1*th +of);  glVertex3f( -dst,  dst, -dst );
        glTexCoord2f(0.75f    , 1*th +of);  glVertex3f( -dst,  dst,  dst );
    glEnd();

    glBegin(GL_QUADS);
        glTexCoord2f(0.50f    , 2*th -of);  glVertex3f( -dst, -dst,  dst );
        glTexCoord2f(0.75f    , 2*th -of);  glVertex3f( -dst, -dst, -dst );
        glTexCoord2f(0.75f    , 1*th +of);  glVertex3f( -dst,  dst, -dst );
        glTexCoord2f(0.50f    , 1*th +of);  glVertex3f( -dst,  dst,  dst );
    glEnd();

    glBegin(GL_QUADS);
        glTexCoord2f(0.00f    , 2*th -of);  glVertex3f(  dst, -dst, -dst );
        glTexCoord2f(0.25f    , 2*th -of);  glVertex3f(  dst, -dst,  dst );
        glTexCoord2f(0.25f    , 1*th +of);  glVertex3f(  dst,  dst,  dst );
        glTexCoord2f(0.00f    , 1*th +of);  glVertex3f(  dst,  dst, -dst );
    glEnd();

    glEndList();
};


void SkyBox::Display(Int tid, Float player_x, Float player_y, Float player_z)
{
    glPushMatrix();
    glTranslatef(player_x, player_y, player_z);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tid);
    glCallList(SkyBox_id);
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}