import UIKit

class ViewController: UIViewController {
    @IBOutlet weak var statusLabel: UILabel!
    var statusTimer: Timer?

    override func viewDidLoad() {
        super.viewDidLoad()

    }

    override func viewWillAppear(_ animated: Bool) {
        if(statusTimer == nil) {
            statusTimer = Timer.scheduledTimer(timeInterval: 2.0, target: self, selector: #selector(ViewController.refresh), userInfo: nil, repeats: true)
            statusTimer?.fire()
        }
    }

    override func viewWillDisappear(_ animated: Bool) {
        if let statusTimer = statusTimer{
            statusTimer.invalidate()
            self.statusTimer = nil
        }
    }

    @IBAction func didTapToggle(_ sender: Any) {
        Request.get("toggle", success: { status in
            print(status)
        }) { error in
            print(error)
        }
    }

    func refresh(){
        Request.get("isClosed", success: { status in
            if let isClosed = status["isClosed"] as? Bool {
                DispatchQueue.main.async {
                    self.statusLabel!.textColor = isClosed ? UIColor.green : UIColor.red
                    self.statusLabel!.text = isClosed ? "Closed" : "Open"
                }
            }
        }, error: { error in
            print(error)
        })
    }
}

