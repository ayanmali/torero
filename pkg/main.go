package main

import (
	"fmt"
	"time"
)

type InferenceRequest struct {
	InputTensors [][]float64
	// Output            []float64
	// ModelID           string
	// ModelVersion      string
	// ModelType         string
	// ModelFormat       string
	// ModelArchitecture string
	// ModelParameters   map[string]string
	// ModelMetadata     map[string]string
}

type Queue[T any] interface {
	Enqueue(message T)
	Dequeue() T
	DequeueMany(count int) []T
	IsEmpty() bool
	Get(i1 int, i2 int) []T
}

type InferenceRequestQueue struct {
	queue []InferenceRequest
}

func (q *InferenceRequestQueue) Enqueue(message InferenceRequest) InferenceRequest {
	q.queue = append(q.queue, message)
	return message
}

func (q *InferenceRequestQueue) Dequeue() InferenceRequest {
	message := q.queue[0]
	q.queue = q.queue[1:]
	return message
}

func (q *InferenceRequestQueue) DequeueMany(count int) []InferenceRequest {
	items := q.queue[:count]
	q.queue = q.queue[count:]
	return items
}

type DynamicBatcher struct {
	maxBatchSize int
	maxWait      time.Duration
	queue        Queue[InferenceRequest]
}

func NewDynamicBatcher(maxBatchSize int, maxWait time.Duration, queue Queue[InferenceRequest]) *DynamicBatcher {
	return &DynamicBatcher{
		maxBatchSize: maxBatchSize,
		maxWait:      maxWait, // e.g. 5 ms
		queue:        queue,
	}
}

func (b *DynamicBatcher) SubmitInferenceRequest(inputTensors [][]float64) InferenceRequest {
	request := InferenceRequest{
		InputTensors: inputTensors,
	}
	b.queue.Enqueue(request)
	return request
}

func (b *DynamicBatcher) DispatchBatch() {
	if b.queue.IsEmpty() {
		return
	}

	batchItems := b.queue.Get(0, b.maxBatchSize)
	b.queue.DequeueMany(len(batchItems))

	// stack input tensors into a single tensor
	stackedInputTensors := make([][]float64, 0)
	for _, item := range batchItems {
		stackedInputTensors = append(stackedInputTensors, item.InputTensors...)
	}

	// make grpc call to python server using stacked input tensors
}

func main() {
	fmt.Println("Hello, World!")
}
