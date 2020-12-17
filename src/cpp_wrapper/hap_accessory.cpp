/*
 * Copyright (c) 2020 Deomid "rojer" Ryabkov
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "hap_accessory.hpp"

namespace wrapper {
namespace hap {

    Accessory::Accessory(
            uint64_t aid,
            HAPAccessoryCategory category,
            const std::string& name,
            const IdentifyCB& identify_cb,
            HAPAccessoryServerRef* server)
        : name_(name)
        , identify_cb_(identify_cb)
        , server_(server)
        , hai_({}) {
        HAPAccessory* a = &hai_.acc;
        a->aid = aid;
        a->category = category;
        a->name = name_.c_str();
        a->manufacturer = "PRODUCT_VENDOR";
        a->model = "PRODUCT_MODEL";
        a->serialNumber = "001122334455";
        a->firmwareVersion = "0.0.1";
        a->hardwareVersion = "PRODUCT_HW_REV";
        a->callbacks.identify = &Accessory::Identify;
        hai_.inst = this;
    }

    Accessory::~Accessory() {
    }

    HAPAccessoryServerRef* Accessory::server() const {
        return server_;
    }
    void Accessory::set_server(HAPAccessoryServerRef* server) {
        server_ = server;
    }

    void Accessory::SetCategory(HAPAccessoryCategory category) {
        hai_.acc.category = category;
    }

    void Accessory::AddService(Service* svc) {
        if (svc == nullptr)
            return;
        svc->set_parent(this);
        AddHAPService(svc->GetHAPService());
    }

    void Accessory::AddService(std::unique_ptr<Service> svc) {
        if (svc == nullptr)
            return;
        AddService(svc.get());
        svcs_.emplace_back(std::move(svc));
    }

    void Accessory::AddHAPService(const HAPService* svc) {
        if (svc == nullptr)
            return;
        if (!hap_svcs_.empty())
            hap_svcs_.pop_back();
        hap_svcs_.push_back(svc);
        hap_svcs_.push_back(nullptr);
        hap_svcs_.shrink_to_fit();
        hai_.acc.services = hap_svcs_.data();
    }

    const HAPAccessory* Accessory::GetHAPAccessory() const {
        if (hap_svcs_.empty())
            return nullptr;
        return &hai_.acc;
    }

    // static
    HAPError Accessory::Identify(
            HAPAccessoryServerRef* server,
            const HAPAccessoryIdentifyRequest* request,
            void* context) {
        auto* ai = reinterpret_cast<const HAPAccessoryWithInstance*>(request->accessory);
        auto* a = ai->inst;
        if (a->identify_cb_ == nullptr) {
            return kHAPError_Unknown;
        }
        return a->identify_cb_(request);
        (void) server;
        (void) context;
    }

} // namespace hap
} // namespace mgos
