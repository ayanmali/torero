using namespace std;

struct InferenceRequest {
    vector<vector<float64>> inputTensors;
}

class Queue {
    vector<InferenceRequest> queue;
    void enqueue(InferenceRequest request) {
        queue.push_back(request);
    }
    vector<InferenceRequest> dequeue(int count) {
        vector<InferenceRequest> requests = queue.sublist(0, count);
        queue.erase(queue.begin(), queue.begin() + count);
        return requests;
    }
    bool isEmpty() {
        return queue.empty();
    }
}

class DynamicBatcher {
    int maxBatchSize;
    int maxWait;
    queue<InferenceRequest> queue;

    DynamicBatcher(int maxBatchSize, int maxWait) : maxBatchSize(maxBatchSize), maxWait(maxWait) {}

    void submitInferenceRequest(vector<vector<float64>> inputTensors) {
        InferenceRequest request = {inputTensors};
        queue.push(request);
    }

    void dispatchBatch() {
        if (queue.isEmpty()) {
            return;
        }
        vector<InferenceRequest> batch = queue.dequeue(maxBatchSize);
        vector<vector<float64>> stackedInputTensors;
        for (InferenceRequest request : batch) {
            stackedInputTensors.insert(stackedInputTensors.end(), request.inputTensors.begin(), request.inputTensors.end());
        }

        // make grpc call to python server using stacked input tensors
    }

}