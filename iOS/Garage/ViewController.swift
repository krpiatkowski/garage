import UIKit

class ViewController: UIViewController {
    @IBOutlet weak var button : UIButton!
    @IBOutlet weak var statusLabel: UILabel!
    @IBOutlet weak var errorLabel: UILabel!
    @IBOutlet weak var activityView: UIActivityIndicatorView!

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
        self.button.isHidden = true
        activityView.isHidden = false
        errorLabel.text = ""
        Request.get("toggle", success: { status in
            DispatchQueue.main.async {
                self.button.isHidden = false
                self.activityView.isHidden = true
            }
            print(status)
        }) { error in
            DispatchQueue.main.async {
                self.button.isHidden = false
                self.activityView.isHidden = true
                self.errorLabel.text = "Failed to toggle!"
            }
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

