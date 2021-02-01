/*
 * Copyright (C) 2020 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_HARDWARE_INTERFACES_NEURALNETWORKS_1_2_UTILS_PREPARED_MODEL_H
#define ANDROID_HARDWARE_INTERFACES_NEURALNETWORKS_1_2_UTILS_PREPARED_MODEL_H

#include <android/hardware/neuralnetworks/1.2/IPreparedModel.h>
#include <android/hardware/neuralnetworks/1.2/types.h>
#include <nnapi/IPreparedModel.h>
#include <nnapi/Result.h>
#include <nnapi/Types.h>
#include <nnapi/hal/CommonUtils.h>
#include <nnapi/hal/ProtectCallback.h>

#include <memory>
#include <tuple>
#include <utility>
#include <vector>

namespace android::hardware::neuralnetworks::V1_2::utils {

class PreparedModel final : public nn::IPreparedModel {
    struct PrivateConstructorTag {};

  public:
    static nn::GeneralResult<std::shared_ptr<const PreparedModel>> create(
            sp<V1_2::IPreparedModel> preparedModel);

    PreparedModel(PrivateConstructorTag tag, sp<V1_2::IPreparedModel> preparedModel,
                  hal::utils::DeathHandler deathHandler);

    nn::ExecutionResult<std::pair<std::vector<nn::OutputShape>, nn::Timing>> execute(
            const nn::Request& request, nn::MeasureTiming measure,
            const nn::OptionalTimePoint& deadline,
            const nn::OptionalTimeoutDuration& loopTimeoutDuration) const override;

    nn::GeneralResult<std::pair<nn::SyncFence, nn::ExecuteFencedInfoCallback>> executeFenced(
            const nn::Request& request, const std::vector<nn::SyncFence>& waitFor,
            nn::MeasureTiming measure, const nn::OptionalTimePoint& deadline,
            const nn::OptionalTimeoutDuration& loopTimeoutDuration,
            const nn::OptionalTimeoutDuration& timeoutDurationAfterFence) const override;

    std::any getUnderlyingResource() const override;

  private:
    nn::ExecutionResult<std::pair<std::vector<nn::OutputShape>, nn::Timing>> executeSynchronously(
            const V1_0::Request& request, MeasureTiming measure) const;
    nn::ExecutionResult<std::pair<std::vector<nn::OutputShape>, nn::Timing>> executeAsynchronously(
            const V1_0::Request& request, MeasureTiming measure) const;

    const sp<V1_2::IPreparedModel> kPreparedModel;
    const hal::utils::DeathHandler kDeathHandler;
};

}  // namespace android::hardware::neuralnetworks::V1_2::utils

#endif  // ANDROID_HARDWARE_INTERFACES_NEURALNETWORKS_1_2_UTILS_PREPARED_MODEL_H
