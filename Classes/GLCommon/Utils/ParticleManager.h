#ifndef __PARTICLE_MANAGER_H__
#define __PARTICLE_MANAGER_H__

#include "cocos2d.h"

USING_NS_CC;

class ParticleManager
{
public:
    
    static ParticleManager* getInstance();
	static void destroyInstance();
    
private:
    
    ParticleManager();
    
    static ParticleManager* m_pInstance;
    /*
    class CGarbo
    {
    public:
        ~CGarbo()
        {
            if (ParticleManager::m_pInstance!= nullptr)
            {
                delete ParticleManager::m_pInstance;
            }
        }
    };
    
    static CGarbo m_garbo;*/
    
public:
    
    std::map<std::string, ValueMap> m_plistMap;
    
    void addPlistData(std::string strPlist,std::string strName);
    
    ValueMap getPlistData(std::string strplist);
};

#endif /* defined(__PARTICLE_MANAGER_H__) */
