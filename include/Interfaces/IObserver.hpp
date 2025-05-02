#ifndef IOBSERVER_HPP_
#define IOBSERVER_HPP_

namespace RayTracer {

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void update(float progress) = 0;
};

}

#endif // IOBSERVER_HPP_