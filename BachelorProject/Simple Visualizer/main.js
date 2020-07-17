
const RADIUS = 0.25
var ind = 0;
const colors = [ 0xFFFFFF, 0xFF0000, 0x00FF00, 0x0000FF ]
var world;
window.onload = function () {
    console.log("Stsrt")
    world = new World();


    world.addAll();

    world.render();

    //window.requestAnimationFrame(iter)
    window.setInterval(iter, 100);
};

function updateProgress(evt) {
}

function iter() {
    //console.log(partString[ind]);
    ind++;
    world.doTurn(ind);
    //window.requestAnimationFrame(iter)
    //window.setInterval(iter, 10000);
}

var configuration = {
    sceneSize: [20, 20, 20],
    kernerFunctionBase: 1,
    d_numOfDims: 3,
    deltaT: 0.0005,
    gravity: new THREE.Vector3(0, -3000, 0),
    glassBounceMultiplier: 0.7
}

class World {
    constructor() {
        this.scene = null;
        this.camera = null; 
        this.renderer = null;
        this.particleMeshList = []
        this.controls = null;

        this.setup()

        }

    //  creates particle and mesh
    addParticle(pos, m_color) {
        let r = RADIUS;
        var geometry = new THREE.SphereGeometry( r, 32, 32 );
        var material = new THREE.MeshLambertMaterial( {color: m_color} );
        var sphere = new THREE.Mesh( geometry, material );
        //sphere.receiveShadow = true;
        //sphere.castShadow = true;
        sphere.position.x = pos.x
        sphere.position.y = pos.y
        sphere.position.z = pos.z
        this.particleMeshList.push(sphere);
        this.scene.add( sphere );
    }

    addAll() {
        var part = partString[0].split(" ");
        for (let i = 0; i < part.length / 4 -1; i++) {
            this.addParticle(new THREE.Vector3(part[4 * i + 0], part[4 * i + 1], part[4 * i + 2]), colors[part[4 * i + 3]]);
        }
    }


    doTurn(id) {    
        if(partString.length<= id) return;
        var part = partString[id].split(" ");
        for (let i = 0; i < part.length / 4 -1; i++) {
            if(i<this.particleMeshList.length) {
                var mesh =  this.particleMeshList[i];
                mesh.position.x = part[4 * i + 0]
                mesh.position.y = part[4 * i + 1]
                mesh.position.z = part[4 * i + 2]
            } else {
                this.addParticle(new THREE.Vector3(part[4 * i + 0], part[4 * i + 1], part[4 * i + 2]), colors[part[4 * i + 3]]);
            }
        }
        this.render()
    }

    //  updates positions of every particle
    //  mesh and particle indexes must be the same
    redrawAllParticles() {
        var minZ = 1000;
        var maxZ = 0;
        for(let i=0; i<this.particleMeshList.length; i++) {
            let particle = this.fluid.particles[i];
            let mesh = this.particleMeshList[i]
            mesh.position.x = particle.position.x;
            mesh.position.y = particle.position.y;
            mesh.position.z = particle.position.z;
            minZ = Math.min(minZ, mesh.position.y);
            maxZ = Math.max(maxZ, mesh.position.y);
            //this.arrows[i].start = particle.position
        }
        // for(let i=0; i<this.particleMeshList.length; i++) {
        //     let mesh = this.particleMeshList[i]
        //     let col = (mesh.position.y - minZ) / (maxZ - minZ)
        //     mesh.material.color.setRGB (1,col,1)
        // }
    }


    setup() {
        if (!Detector.webgl) Detector.addGetWebGLMessage();
        this.scene = new THREE.Scene();

        this.camera = new THREE.PerspectiveCamera(60, window.innerWidth / window.innerHeight, 1, 10000);
        this.camera.position.z = 20
        this.camera.position.x = 5;
        this.camera.position.y = 10;
        this.camera.lookAt(new THREE.Vector3(0, 0, 0));


        this.controls = new THREE.OrbitControls(this.camera);
        this.controls.addEventListener('change', this.onEvent);

        this.renderer = new THREE.WebGLRenderer();
        this.renderer.setClearColor("#ffffff");
        this.renderer.setSize(window.innerWidth * 0.95, window.innerHeight * 0.92);

        // Resizing window
        //THREEx.WindowResize(this.renderer, this.camera);
        document.body.appendChild(this.renderer.domElement);

        var geometry = new THREE.BoxGeometry(1, 0.5, 0.2);
        var material = new THREE.MeshBasicMaterial({ color: "#00ff00" });
        var cube = new THREE.Mesh(geometry, material);
        this.scene.add(cube);
        //cube.position.set(configuration.sceneSize[0]/2, configuration.sceneSize[1]/2, configuration.sceneSize[2]/2)

        // LIGHT

        this.renderer.setClearColor(0xbbbbb1, 1);
        this.renderer.shadowMapEnabled = true;
        ;
        var ambiColor = "#000000";
        var ambientLight = new THREE.AmbientLight(ambiColor, 5);
        this.scene.add(ambientLight)

        var target = new THREE.Object3D();
        //target.position.set(new THREE.Vector3(configuration.sceneSize[0], 0, configuration.sceneSize[2]));
        target.position.x = 20;
        target.position.y = 1;
        target.position.z = 20;

        var pointColor = "#ffffff";
        var spotLight = new THREE.SpotLight(pointColor);
        spotLight.position.x = 0;
        spotLight.position.y = 25;
        spotLight.position.z = 1;
        spotLight.castShadow = true;
        spotLight.angle = 1;
        //spotLight.target = target;
        spotLight.shadowBias = 0.004;
        spotLight.distance = 0;



        this.scene.add(spotLight);



        //window.addEventListener( 'resize', this.onWindowResize, false ); // ????????????
        window.addEventListener('keypress', World.onKeyPress);
        this.renderer.render(this.scene, this.camera);
        window.addEventListener('keypress', this.onKeyPress, false);
    };

    render() {
        this.renderer.render(this.scene, this.camera);
    }

    onWindowResize() {

        this.camera.aspect = window.innerWidth / window.innerHeight;
        this.camera.updateProjectionMatrix();

        this.renderer.setSize( window.innerWidth, window.innerHeight );

        //this.controls.handleResize();
    }

    static onKeyPress(event) {
        var glassMoveAngleDiv = 100;    // szklanka obraca się o kąt (Math.PI / glassMoveAngleDiv)
        var glassMoveDistance = 0.005;
        switch (event.code) {
            
            case "KeyQ":
                world.camera.rotation.z += 0.05;
                break;
            case "KeyE":
                world.camera.rotation.z -= 0.05;
                break;
            case "KeyZ":
                world.camera.zoom *= 1.05;
                world.camera.updateProjectionMatrix();
                break;
            case "KeyX":
                world.camera.zoom /= 1.05;
                world.camera.updateProjectionMatrix();
                break;
            case "KeyI":
                world.camera.position.y += 5;
                break;
            case "KeyJ":
                world.camera.position.x -= 5;
                break;
            case "KeyK":
                world.camera.position.y -= 5;
                break;
            case "KeyL":
                world.camera.position.x += 5;
                break;            
            case "KeyN":
                world.camera.position.z -= 5;
                break;
            case "KeyM":
                world.camera.position.z += 5;
                break;

            case "Digit1":
                mug.rotatateAxis(new THREE.Vector3(1, 0, 0), Math.PI / glassMoveAngleDiv)
                break
            case "Digit2":
                mug.rotatateAxis(new THREE.Vector3(1, 0, 0), -Math.PI / glassMoveAngleDiv)
                break
            case "Digit3":
                mug.rotatateAxis(new THREE.Vector3(0, 1, 0), Math.PI / glassMoveAngleDiv)
                break
            case "Digit4":
                mug.rotatateAxis(new THREE.Vector3(0, 1, 0), -Math.PI / glassMoveAngleDiv)
                break
            case "Digit5":
                mug.rotatateAxis(new THREE.Vector3(0, 0, 1), Math.PI / glassMoveAngleDiv)
                break
            case "Digit6":
                mug.rotatateAxis(new THREE.Vector3(0, 0, 1), -Math.PI / glassMoveAngleDiv)
                break
            case "KeyW":
                mug.move(new THREE.Vector3(glassMoveDistance, 0, 0))
                break
            case "KeyS":
                mug.move(new THREE.Vector3(-glassMoveDistance ,0 , 0))
                break
            case "KeyA":
                mug.move(new THREE.Vector3(0, 0, -glassMoveDistance))
                break
            case "KeyD":
                mug.move(new THREE.Vector3(0, 0, glassMoveDistance))
                break
            case "KeyR":
                mug.move(new THREE.Vector3(0, -glassMoveDistance, 0))
                break
            case "KeyF":
                mug.move(new THREE.Vector3(0, glassMoveDistance, 0))
                break

        }
        world.renderer.render(world.scene, world.camera);
    }

    onEvent(){
        world.renderer.render(world.scene, world.camera);
    }

    onMouseWheelChange(event) {
        //
    }
}
//                   X    Y   Z
