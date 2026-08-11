#pragma once
// #include <vector>
#include <span>
#include "../config.hpp"
#define DEBUG
#ifdef DEBUG
#include <iostream>
#endif

using InputTensors = std::array<std::array<float16_t, MAX_REQ_WIDTH>, MAX_REQ_HEIGHT>;

struct InferenceRequest {
    InputTensors input_tensors;
};

struct Queue {
    public:

    std::array<InferenceRequest, MAX_QUEUE_SIZE> reqs;
    size_t cur_idx = 0;

    bool IsEmpty() {
        return cur_idx == 0;
    }
    bool IsFull() {
        return cur_idx + 1 == MAX_QUEUE_SIZE;
    }

    bool Emplace(InferenceRequest&& request) {
        if (IsFull()) {
            #ifdef DEBUG
            std::cerr << "Queue is full\n";
            #endif
            return false;
        }
        reqs[cur_idx++] = std::forward<InferenceRequest>(request);
        return true;
    }
    // InferenceRequest dequeue() {
    //     if (isEmpty()) {
    //         throw std::runtime_error("Queue is empty");
    //     }
    //     InferenceRequest request = queue.front();
    //     queue.erase(queue.begin());
    //     return request;
    // }

    std::optional<std::span<InferenceRequest>> Dequeue(int count) {
        if (count > cur_idx) {
            #ifdef DEBUG
            std::cerr << "Queue is empty\n";
            #endif
            return {};
        }
        auto requests = std::span<InferenceRequest>(
            reqs.begin(), reqs.end()
        ).first(count);
        cur_idx -= count;
        return requests;
    }
};

struct DynamicBatcher {
    Queue queue_;
    int max_batch_size_;
    int max_wait_;

    DynamicBatcher(Queue queue, int max_batch_size, int max_wait)
    : queue_(queue), max_batch_size_(max_batch_size), max_wait_(max_wait) {}

    void SubmitInferenceRequest(InputTensors&& input_tensors) {
        queue_.Emplace(InferenceRequest{
            std::forward<InputTensors>(input_tensors)
        });
    }

    void DispatchBatch() {
        if (queue_.IsEmpty()) {
            return;
        }
        std::optional<std::span<InferenceRequest>> batch_opt = queue_.Dequeue(max_batch_size_);
        if (!batch_opt) {
            #ifdef DEBUG
            std::cerr << "Queue batch is empty\n";
            #endif
            return;
        }

        std::span<InferenceRequest> batch = batch_opt.value();

        // stack input tensors into a single tensor
        // std::vector<std::vector<float16_t> > stacked_input_tensors{};
        // for (InferenceRequest request : batch) {
        //     stacked_input_tensors.insert(stacked_input_tensors.end(), request.input_tensors.begin(), request.input_tensors.end());
        // }

        // make grpc call to python server using stacked input tensors
    }
};
