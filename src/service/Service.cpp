#include "Service.h"
#include "ServiceImpl.h"
bool CService::start()
{
    return m_impl->start();
}

void CService::stop()
{
    return m_impl->stop();

}
void CService::wait()
{
    m_impl->wait();
}

CService::CService()
{
    m_impl = std::make_unique<CServiceImpl>(this);
}

CService::~CService()
{
    stop();
    wait();
}
