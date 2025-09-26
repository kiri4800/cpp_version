//#include "Model.h"
//#include "darknet.h"
//#include <torch/script.h>
//// Функция для преобразования cv::Mat в torch::Tensor
//torch::Tensor cvMatToTensor(const cv::Mat& image) {
//    // 1. Конвертируем BGR -> RGB
//    cv::Mat rgbImage;
//    cv::cvtColor(image, rgbImage, cv::COLOR_BGR2RGB);
//
//    // 2. Меняем dimensions [H, W, C] -> [C, H, W]
//    cv::Mat floatImage;
//    rgbImage.convertTo(floatImage, CV_32FC3, 1.0 / 255.0); // Нормализуем к [0, 1]
//    auto tensor = torch::from_blob(floatImage.data, {rgbImage.rows, rgbImage.cols, 3}, torch::kFloat);
//    tensor = tensor.permute({2, 0, 1}); // [C, H, W]
//
//    // 3. Добавляем batch dimension [C, H, W] -> [1, C, H, W]
//    tensor = tensor.unsqueeze(0);
//
//    // 4. Нормализация вручную (замена torch::data::transforms::Normalize)
//    // Mean и std для ImageNet
//    torch::Tensor mean = torch::tensor({0.485, 0.456, 0.406}).view({1, 3, 1, 1});
//    torch::Tensor std = torch::tensor({0.229, 0.224, 0.225}).view({1, 3, 1, 1});
//
//    // Нормализация: (tensor - mean) / std
//    tensor = (tensor - mean) / std;
//
//    return tensor.clone(); // Важно сделать копию данных
//}
//
//Model::Model(const std::string& path){
//    module = torch::jit::load(path);
//}
//#include <opencv2/opencv.hpp>
//#include <torch/torch.h>
//
//// Функция для конвертации torch::Tensor (маски) в cv::Mat
//cv::Mat tensorToMask(const torch::Tensor& tensor, bool normalize = true) {
//    // 1. Убедимся, что тензор находится на CPU
//    torch::Tensor cpu_tensor = tensor.to(torch::kCPU);
//
//    // 2. Уберем размерности batch и channel, если они есть
//    torch::Tensor squeezed = cpu_tensor.squeeze();
//
//    // 3. Конвертируем в подходящий тип данных
//    torch::Tensor final_tensor;
//    if (normalize) {
//        // Если тензор содержит значения в диапазоне [0, 1], масштабируем до [0, 255]
//        final_tensor = squeezed.mul(255).clamp(0, 255).to(torch::kU8);
//    } else {
//        // Если значения уже в правильном диапазоне [0, 255]
//        final_tensor = squeezed.to(torch::kU8);
//    }
//
//    // 4. Получаем размеры тензора
//    int height = final_tensor.size(0);
//    int width = final_tensor.size(1);
//
//    // 5. Создаем cv::Mat и копируем данные
//    cv::Mat mask(height, width, CV_8UC1);
//    std::memcpy(mask.data, final_tensor.data_ptr(), sizeof(torch::kU8) * height * width);
//
//    return mask;
//}
//void Model::test(const std::string& pathToImage){
//    cv::Mat image = cv::imread(pathToImage);
//    auto input_tensor = cvMatToTensor(image);
//    // Создаем входные данные для модели (зависит от экспортированной модели)
//    // ВАЖНО: Это самая сложная часть. Нужно точно воспроизвести тот формат ввода,
//    // который ожидает модель (тензоры с промтами, etc.).
//    std::vector<torch::jit::IValue> inputs;
//    inputs.push_back(input_tensor);
//    // inputs.push_back(torch::tensor({{1024, 1024}})); // Пример промта (координаты)
//
//    // Выполняем инференс
//    torch::NoGradGuard no_grad; // Отключаем расчет градиентов для скорости
//    auto output = module.forward(inputs).toTuple();
//
//    // Обрабатываем вывод (сильно зависит от вывода модели)
//    // output будет содержать тензоры с масками, скорами и т.д.
//    torch::Tensor masks = output->elements()[0].toTensor();
//    masks = masks.squeeze().detach().cpu(); // Убираем batch dim, переносим на CPU
//    auto image1 = tensorToMask(masks);
//    cv::imshow("lolkek",image1);
//}