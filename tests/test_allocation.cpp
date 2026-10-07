#include <array>
#include <cstdlib>
#include <new>

#if defined(_MSC_VER)
#include <malloc.h>
#endif

#include "../examples/spatial/SpatialIndex.hpp"

namespace
{
thread_local bool tracking = false;
thread_local std::size_t allocations = 0;
void count() noexcept
{
    if (tracking)
        ++allocations;
}
void* allocate(std::size_t bytes)
{
    count();
    if (auto* memory = std::malloc(bytes == 0 ? 1 : bytes))
        return memory;
    throw std::bad_alloc{};
}
void* allocateAligned(std::size_t bytes, std::size_t alignment)
{
    count();
#if defined(_MSC_VER)
    auto* memory = _aligned_malloc(bytes == 0 ? 1 : bytes, alignment);
#else
    void* memory = nullptr;
    if (posix_memalign(&memory, alignment, bytes == 0 ? 1 : bytes) != 0)
        memory = nullptr;
#endif
    if (memory)
        return memory;
    throw std::bad_alloc{};
}
void freeAligned(void* memory) noexcept
{
#if defined(_MSC_VER)
    _aligned_free(memory);
#else
    std::free(memory);
#endif
}
} // namespace

void* operator new(std::size_t bytes) { return allocate(bytes); }
void* operator new[](std::size_t bytes) { return allocate(bytes); }
void operator delete(void* memory) noexcept { std::free(memory); }
void operator delete[](void* memory) noexcept { std::free(memory); }
void operator delete(void* memory, std::size_t) noexcept { std::free(memory); }
void operator delete[](void* memory, std::size_t) noexcept { std::free(memory); }
void* operator new(std::size_t bytes, std::align_val_t alignment)
{
    return allocateAligned(bytes, static_cast<std::size_t>(alignment));
}
void* operator new[](std::size_t bytes, std::align_val_t alignment)
{
    return allocateAligned(bytes, static_cast<std::size_t>(alignment));
}
void operator delete(void* memory, std::align_val_t) noexcept { freeAligned(memory); }
void operator delete[](void* memory, std::align_val_t) noexcept { freeAligned(memory); }
void operator delete(void* memory, std::size_t, std::align_val_t) noexcept { freeAligned(memory); }
void operator delete[](void* memory, std::size_t, std::align_val_t) noexcept
{
    freeAligned(memory);
}

int main()
{
    using namespace sub0hexgrid;
    const auto layout = *PointyLayout::tryCreate(1);
    const auto region = *AxialRegion::tryCreate({-10, -10}, {10, 10});
    std::array<Point, 128> points{};
    example::SpatialIndex index{layout, region, points.size()};
    std::array<Axial, 7> cells{};
    std::array<std::size_t, 7> hits{};
    tracking = true;
    for (int round = 0; round < 4; ++round) {
        if (!index.beginRebuild(points))
            return 1;
        while (index.stepRebuild(7) == example::SpatialIndex::BuildState::working) {
        }
        const auto candidates = CandidateCells::tryCreate(layout, region, {}, 5);
        if (!candidates)
            return 2;
        CandidateCursor cursor{*candidates};
        while (!cursor.isDone()) {
            const auto count = cursor.read(cells);
            for (std::size_t i = 0; i < count; ++i)
                if (!region.tryCell(*region.tryIndex(cells[i])))
                    return 3;
        }
        auto query = index.tryQuery({}, 0);
        if (!query)
            return 4;
        std::size_t count = 0;
        for (;;) {
            const auto batch = query->read(hits, 3);
            if (!batch.valid_)
                return 5;
            count += batch.written_;
            if (batch.done_)
                break;
        }
        if (count != points.size())
            return 6;
    }
    tracking = false;
    return allocations == 0 ? 0 : 7;
}
