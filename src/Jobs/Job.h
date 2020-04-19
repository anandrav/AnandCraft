/*
 * NOTE: Jobs should not influence the state of the simulation.
 * Use Events for that.
 */

#pragma once

#include <functional>

using Job_t = std::function<void (void)>;