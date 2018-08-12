//
//  Requests.swift
//  Garage
//
//  Created by Krzysztof Piatkowski on 22/07/2017.
//  Copyright © 2017 kpi. All rights reserved.
//

import Foundation

class Request {
    private static let base64LoginString = "\(API_USER):\(API_PASSWORD)".data(using: String.Encoding.utf8)!.base64EncodedString()
    static func get(_ path : String, success: @escaping (NSDictionary) -> Void, error: @escaping (Error) -> Void) {
        let request = createRequest(path, method: "GET")
        handleRequest(request: request, success: success, error: error)
    }

    private static func createRequest(_ path:String, method:String) -> URLRequest {
        let url = URL(string: "\(API_URL)/\(path)")
        var request = URLRequest(url: url!)
        request.httpMethod = method
        request.setValue("Basic \(base64LoginString)", forHTTPHeaderField: "Authorization")

        return request
    }

    private static func handleRequest(request: URLRequest, success: @escaping (NSDictionary) -> Void, error: @escaping (Error) -> Void){
        let task = URLSession.shared.dataTask(with: request) {
            data, response, err in

            if let err = err {
                error(err)
            } else {
                do {
                    let json = try JSONSerialization.jsonObject(with: data!, options: .mutableContainers) as! NSDictionary

                    success(json)
                } catch let err {
                    error(err)
                }
            }

        }
        task.resume();
    }
}
